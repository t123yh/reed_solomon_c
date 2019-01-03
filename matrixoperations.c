//
// Created by t123yh on 18-12-31.
//

#include <asm/errno.h>
#include "matrixoperations.h"
#include "galois.h"
#include "mem.h"

// mostly ported from https://github.com/klauspost/reedsolomon/


void rs_init_vandermonde(unsigned char *matrix, unsigned char rows, unsigned char cols)
{
    unsigned char r, c;
    
    for (r = 0; r < rows; r++)
    {
        for (c = 0; c < cols; c++)
        {
            matrix[r * cols + c] = rs_gal_exp(r, c);
        }
    }
}

static void rs_init_identity_matrix(unsigned char *matrix, unsigned char n)
{
    unsigned char i;
    
    memset(matrix, 0, n * n);
    for (i = 0; i < n; i++)
        matrix[i * n + i] = 1;
}

#define SWAP(A, B) { typeof(A) __swap_tmp = (A); (A) = (B); (B) = __swap_tmp; }

/* use gaussian elimination to convert (l, r) to row reduced form. */
static void rs_gaussian_elimination(unsigned char *left, unsigned char *right, unsigned char n)
{
    int r, rb, i;
    unsigned char tmp;
    
    for (r = 0; r < n; r++)
    {
        /* If the element on the diagonal is 0, find a row below
		 * that has a non-zero and swap them. */
        if (unlikely(left[r * n + r] == 0))
        {
            for (rb = r + 1; rb < n; rb++)
            {
                if (likely(left[rb * n + r] != 0))
                {
                    /* swap row */
                    for (i = 0; i < n; i++)
                    {
                        SWAP(left[r * n + i], left[rb * n + i]);
                        SWAP(right[r * n + i], right[rb * n + i]);
                    }
                    break;
                }
            }
        }
        
        /* We ignore the singular matrix check here,
         * for that we won't encounter singular matrix in our code. */
        
        /* check for singular matrix */
        /*
        if (unlikely(left[r * n + r] == 0))
            return -EINVAL;
        */
        
        /* Scale the diagonal element to 1 */
        tmp = rs_gal_divide(1, left[r * n + r]);
        for (i = 0; i < n; i++)
        {
            left[r * n + i] = rs_gal_multiply(left[r * n + i], tmp);
            right[r * n + i] = rs_gal_multiply(right[r * n + i], tmp);
        }
        
        /* Make everything below the 1 be a 0 by subtracting
		 * a multiple of it.  (Subtraction and addition are
		 * both exclusive or in the Galois field.) */
        for (rb = r + 1; rb < n; rb++)
        {
            if (left[rb * n + r] != 0)
            {
                tmp = left[rb * n + r];
                for (i = 0; i < n; i++)
                {
                    left[rb * n + i] ^= rs_gal_multiply(tmp, left[r * n + i]);
                    right[rb * n + i] ^= rs_gal_multiply(tmp, right[r * n + i]);
                }
            }
        }
    }
    
    /* Now clear the part above the main diagonal. */
    for (r = 0; r < n; r++)
    {
        for (rb = 0; rb < r; rb++)
        {
            if (likely(left[rb * n + r] != 0))
            {
                tmp = left[rb * n + r];
                for (i = 0; i < n; i++)
                {
                    left[rb * n + i] ^= rs_gal_multiply(tmp, left[r * n + i]);
                    right[rb * n + i] ^= rs_gal_multiply(tmp, right[r * n + i]);
                }
            }
        }
    }
    /* return 0; */
}


void rs_invert_square_matrix(const unsigned char *mat, unsigned char *result, unsigned char n)
{
    unsigned char tmp[MAX_MATRIX_SIZE];
    memcpy(tmp, mat, n * n);
    
    rs_init_identity_matrix(result, n);
    rs_gaussian_elimination(tmp, result, n);
}

void rs_matrix_multiply(
        const unsigned char *mat_a,
        const unsigned char *mat_b,
        unsigned char *result,
        unsigned char a_row,
        unsigned char a_col, /* == b_row */
        unsigned char b_col
)
{
    unsigned char r, c, i, val;
    for (r = 0; r < a_row; r++)
    {
        for (c = 0; c < b_col; c++)
        {
            val = 0;
            for (i = 0; i < a_col; i++)
            {
                val ^= rs_gal_multiply(mat_a[r * a_col + i], mat_b[i * b_col + c]);
            }
            result[r * b_col + c] = val;
        }
    }
}