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
    rs_init(&rs, 10, 3);
    
    printMat(rs.base_matrix, 13, 10);
}
