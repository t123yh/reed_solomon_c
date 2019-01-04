//
// Created by t123yh on 19-1-1.
//

#ifndef _WG_GALOIS_H
#define _WG_GALOIS_H

#include "common.h"

uint8_t rs_gal_multiply(uint8_t a, uint8_t b);
uint8_t rs_gal_exp(uint8_t a, int n);
uint8_t rs_gal_divide(uint8_t a, uint8_t b);
void rs_gal_mul_slice_xor(uint8_t c, const uint8_t *in, uint8_t *out, int n);

#endif /* _WG_GALOIS_H */
