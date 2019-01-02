//
// Created by t123yh on 19-1-1.
//

#ifndef _WG_GALOIS_H
#define _WG_GALOIS_H

unsigned char rs_gal_multiply(unsigned char a, unsigned char b);
unsigned char rs_gal_exp(unsigned char a, int n);
unsigned char rs_gal_divide(unsigned char a, unsigned char b);
void rs_gal_mul_slice_xor(unsigned char c, const unsigned char *in, unsigned char *out, int n);

#endif /* _WG_GALOIS_H */
