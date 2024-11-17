#pragma once
#include "obn.h"

void gpu_mod_exp(OURBIGNUM* r, const OURBIGNUM* a, const OURBIGNUM* p, const OURBIGNUM* m, OBN_MUL_GPU_CTX *ctx);