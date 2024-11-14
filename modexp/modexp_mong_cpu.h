#pragma once
#include "obn.h"
#include "config.h"

void REDC(OURBIGNUM* T, OURBIGNUM* result, const BN_CONFIG* bn_config);
void toMontgomeryForm(const OURBIGNUM* a, OURBIGNUM* result, const BN_CONFIG& bn_config);
void fromMontgomeryForm(const OURBIGNUM* a, OURBIGNUM* result, const BN_CONFIG& bn_config);
void mont_mul(const OURBIGNUM* a, const OURBIGNUM* b, OURBIGNUM* result, const BN_CONFIG& bn_config);

void mont_mod_exp(OURBIGNUM* r, const OURBIGNUM* a, const OURBIGNUM* p, const BN_CONFIG& bn_config);