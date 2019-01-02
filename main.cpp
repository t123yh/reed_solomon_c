#include <stdio.h>
#include <math.h>
#include "reedsolomon.h"
#include "matrixoperations.h"

void printMat(unsigned char *mat, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%3d ", (int) mat[i * col + j]);
        }
        printf("\n");
    }
}

int main()
{
    reed_solomon rs;
    rs_init(&rs, 2, 2);
    
    unsigned char r[4][3] = {
            {0, 1, 2},
            {3, 4, 5},
            {0, 0, 0},
            {0, 0, 0}
    };
    unsigned char *dat[2] = {r[0], r[1]};
    unsigned char *parity[2] = {r[2], r[3]};
    rs_encode(&rs, dat, parity, 3);
}
