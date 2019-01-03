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
    
    unsigned char d[13][1024];
    
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 1024; j++)
        {
            d[i][j] = rand();
        }
    }
    
    unsigned char *ds[13];
    for (int i = 0; i < 13; i++)
    {
        ds[i] = d[i];
    }
    
    for (int i = 0; i < 1000000; i++)
        rs_encode(&rs, ds, 3);
    printf("%d", ds[10][0]);
}
