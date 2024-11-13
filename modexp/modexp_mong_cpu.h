#pragma once
#include "utils/obn.h"
#include "utils/config.h"

void REDC(OURBIGNUM& T, OURBIGNUM& result, const BN_CONFIG& bn_config);
void toMontgomeryForm(const OURBIGNUM& a, OURBIGNUM& result, const BN_CONFIG& bn_config);
void fromMontgomeryForm(const OURBIGNUM& a, OURBIGNUM& result, const BN_CONFIG& bn_config);
void mont_mul(const OURBIGNUM& a, const OURBIGNUM& b, OURBIGNUM& result, const BN_CONFIG& bn_config);
void mont_mod_exp(const OURBIGNUM& base, const OURBIGNUM& exp, OURBIGNUM& result, const BN_CONFIG& bn_config);