#include <cuda_runtime.h>


/* c: carry (may increment by 1)
   s: partial sum
   x, y: operands */
#define ADD_CARRY(c, s, x, y) \
		do { \
			WORD _t = (x) + (y); \
			(c) += (_t < (x)); \
			(s) = _t; \
		} while (0)

/* Same with ADD_CARRY, but sets y to 0 */
#define ADD_CARRY_CLEAR(c, s, x, y) \
		do { \
			WORD _t = (x) + (y); \
			(y) = 0; \
			(c) += (_t < (x)); \
			(s) = _t; \
		} while (0)

/* b: borrow (may increment by 1)
   d: partial difference
   x, y: operands (a - b) */
#define SUB_BORROW(b, d, x, y) \
		do { \
			WORD _t = (x) - (y); \
			(b) += (_t > (x)); \
			(d) = _t; \
		} while (0)

/* Same with SUB_BORROW, but sets y to 0 */
#define SUB_BORROW_CLEAR(b, d, x, y) \
		do { \
			WORD _t = (x) - (y); \
			(y) = 0; \
			(b) += (_t > (x)); \
			(d) = _t; \
		} while (0)

// Return most significant WORD
__device__ WORD mp_umul_hi(WORD a, WORD b) {
#ifdef USE_64BIT
	return __umul64hi(a, b);
#else
	return __umulhi(a, b);
#endif
}
// Return least significant WORD
__device__ WORD mp_umul_lo(WORD a, WORD b) {
	return a * b;
}


// Montgomery multiplication
__device__ void mp_montmul_dev(WORD *ret, WORD *ar, WORD *br,
	int limb_idx, int idx, int msg_size, WORD *n, WORD np) {
	__shared__ WORD _t[2*BLK_SIZE];
	__shared__ WORD _c[2*BLK_SIZE];

	volatile WORD *t = _t + 2 * msg_size * limb_idx;
	volatile WORD *c = _c + 2 * msg_size * limb_idx;

	c[idx] = 0;
	c[idx + msg_size] = 0;
	t[idx] = 0;
	t[idx + msg_size] = 0;

	for (int i=0; i<msg_size; i++) {
		WORD hi = mp_umul_hi(ar[i], br[idx]);
		WORD lo = mp_umul_lo(ar[i], br[idx]);

		ADD_CARRY(c[i+idx+1], t[i+idx+1], t[i+idx+1], hi);
		ADD_CARRY(c[i+idx], t[i+idx], t[i+idx], lo);

		WORD m = t[i] * np;
		hi = mp_umul_hi(m, n[idx]);
		lo = mp_umul_lo(m, n[idx]);

		ADD_CARRY(c[idx+i+1], t[idx+i+1], t[idx+i+1], hi);
		ADD_CARRY(c[idx+i], t[idx+i], t[idx+i], lo);
		ADD_CARRY_CLEAR(c[idx+i+1], t[idx+i+1], t[idx+i+1], c[idx+i]);
	}

	/* here all t[0] ~ t[msg_size - 1] should be zero. c too */
	while (__any(c[idx + msg_size - 1] != 0))
		ADD_CARRY_CLEAR(c[idx + msg_size], t[idx + msg_size], 
			t[idx + msg_size], c[idx + msg_size - 1]);

	/* step 2: return t or t - n */
	if (c[msg_size * 2 - 1])		// c may be 0 or 1, but not 2
		goto u_is_bigger;

	/* Ugly, but practical.
	 * Can we do this much better with Fermi's ballot()? */
	for (int i = msg_size - 1; i >= 0; i--) {
		if (t[i + msg_size] > n[i])
			goto u_is_bigger;
		if (t[i + msg_size] < n[i])
			goto n_is_bigger;
	}

u_is_bigger:
	/* return t - n. Here, c is used for borrow */
	SUB_BORROW(c[idx], ret[idx], t[idx + msg_size], n[idx]);

	if (idx < msg_size - 1) {
		while (__any(c[idx] != 0)) {
			SUB_BORROW_CLEAR(c[idx + 1], ret[idx + 1],
					ret[idx + 1], c[idx]);
		}
	}
	return;

n_is_bigger:
	/* return t */
	ret[idx] = t[idx + msg_size];
	return;
}

// Exponentiate msg with private key, including Montgomerization and deMontgomerization
__global__ void gpu_modexp(
	int msg_num, int msg_size, WORD *input, WORD *output) {
	__shared__ WORD _ret[BLK_SIZE];
	__shared__ WORD _tmp[BLK_SIZE];

	const int limb_idx = threadIdx.x / msg_size;
	const int msg_idx = blockIdx.x * blockDim.x / msg_size + limb_idx;
	const int idx = threadIdx.x % msg_size;

	if(msg_idx >= msg_num) return;

	WORD *ret = _ret + limb_idx * msg_size;
	WORD *tmp = _tmp + limb_idx * msg_size;

	tmp[idx] = input[msg_idx * msg_size + idx];
	ret[idx] = r_sqr_d[idx];

	mp_montmul_dev(ret, ret, tmp, limb_idx, idx, msg_size, n_d, np_d[0]); /* ret = ar */
	tmp[idx] = ret[idx];

	int t = msg_size * BITS_PER_WORD - 1; /* bit index of d_d */
	while (((d_d[t/BITS_PER_WORD] >> (t%BITS_PER_WORD)) & 1) == 0 && t>0)
		t--;
	t--;

	while (t >= 0) {
		mp_montmul_dev(ret, ret, ret, limb_idx, idx, msg_size, n_d, np_d[0]);
	
		if (((d_d[t/BITS_PER_WORD] >> (t%BITS_PER_WORD)) & 1) == 1) {
			mp_montmul_dev(ret, ret, tmp, limb_idx, idx, msg_size, n_d, np_d[0]);
		}
		t--;
	}

	/* ret = (a^e)*r; calculate a^e = montmul(ret, 1) */
	tmp[idx] = (idx==0);
	mp_montmul_dev(ret, ret, tmp, limb_idx, idx, msg_size, n_d, np_d[0]);

	output[msg_idx*msg_size + idx] = ret[idx];
}