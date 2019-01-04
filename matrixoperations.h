//
// Created by t123yh on 18-12-31.
//

#ifndef _WG_MATRIXOPERATIONS_H
#define _WG_MATRIXOPERATIONS_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void rs_init_vandermonde(uint8_t *matrix, uint8_t rows, uint8_t cols);

void rs_invert_square_matrix(const uint8_t *mat, uint8_t *target, uint8_t n);

void rs_matrix_multiply(
        const uint8_t *mat_a,
        const uint8_t *mat_b,
        uint8_t *result,
        uint8_t a_row,
        uint8_t a_col, /* == b_row */
        uint8_t b_col
);

#ifdef __cplusplus
};
#endif

#endif /* _WG_MATRIXOPERATIONS_H */
