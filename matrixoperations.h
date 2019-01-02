//
// Created by t123yh on 18-12-31.
//

#ifndef _WG_MATRIXOPERATIONS_H
#define _WG_MATRIXOPERATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

void rs_init_vandermonde(unsigned char *matrix, unsigned char rows, unsigned char cols);

int rs_invert_square_matrix(const unsigned char *mat, unsigned char *target, unsigned char n);

void rs_matrix_multiply(
        const unsigned char *mat_a,
        const unsigned char *mat_b,
        unsigned char *result,
        unsigned char a_row,
        unsigned char a_col, /* == b_row */
        unsigned char b_col
);

#ifdef __cplusplus
};
#endif

#endif /* _WG_MATRIXOPERATIONS_H */
