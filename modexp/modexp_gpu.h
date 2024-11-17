#pragma once
#include "obn.h"

void mod_exp_gpu(OURBIGNUM* r, const OURBIGNUM* a, const OURBIGNUM* p, const OURBIGNUM* m, OBN_MUL_GPU_CTX *ctx);