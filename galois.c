//
// Created by t123yh on 19-1-1.
//

#include "galois.h"
#include "galoisconstants.h"

unsigned char rs_gal_multiply(unsigned char a, unsigned char b)
{
    return gal_mul_table[a][b];
}

unsigned char rs_gal_exp(unsigned char a, int n)
{
    if (n == 0)
        return 1;
    
    if (a == 0)
        return 0;
    
    int logA = gal_log_table[a] * n;
    
    // TODO: Can we use mod here?
    while (logA > 255)
        logA -= 255;
    
    return gal_exp_table[logA];
}

unsigned char rs_gal_divide(unsigned char a, unsigned char b)
{
    int result;
    
    result = gal_log_table[a] - gal_log_table[b];
    if (result < 0)
        result += 255;
    return gal_exp_table[result];
}

void rs_gal_mul_slice_xor(unsigned char c, const unsigned char *in, unsigned char *out, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        out[i] ^= rs_gal_multiply(c, in[i]);
    }
}
