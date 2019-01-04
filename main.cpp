#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include "reedsolomon.h"
#include "radix-tree.h"
#include "matrixoperations.h"

void printMat(uint8_t *mat, int row, int col)
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

void p(bool x)
{
    if (x)
        printf("true\n");
    else
        printf("false\n");
}

int main()
{
    reed_solomon rs;
    rs_init(&rs, 10, 3);
    
    // shard size is arbitrary
    const int shardSize = 997;
    
    // original 10 pieces of data can be recovered if any 10 of the 13 shards is preserved.
    const int dataShards = 10, parityShards = 3, totalShards = dataShards + parityShards;
    
    // store data
    uint8_t d[totalShards][shardSize];
    
    srand(time(NULL));
    for (int i = 0; i < dataShards; i++)
    {
        for (int j = 0; j < shardSize; j++)
        {
            d[i][j] = rand();
        }
    }
    
    uint8_t *ds[totalShards];
    for (int i = 0; i < totalShards; i++)
    {
        ds[i] = d[i];
    }
    
    // 11 is a parity shard
    const int shardsToTest[] = {2, 3, 11};
    
    // encode parity info
    rs_encode(&rs, ds, shardSize);
    
    std::cout << "Shard size: " << shardSize << std::endl;
    for (auto n : shardsToTest)
    {
        std::cout << "Original shard " << n << " data: ";
        std::ios_base::fmtflags f(std::cout.flags()); // save flags
        for (int i = 0; i < shardSize; i++)
        {
            std::cout << std::setfill('0') << std::setw(2) << std::hex << (int) ds[n][i] << " ";
        }
        std::cout.flags(f); // restore flags
        std::cout << std::endl;
    }
    
    unsigned long validShards = (1 << totalShards) - 1;
    for (auto n : shardsToTest)
    {
        std::cout << "Clearing shard " << n << " data" << std::endl;
        memset(ds[n], 0, shardSize);
        __clear_bit(n, &validShards);
    }
    std::cout << "Reconstructing..." << std::endl;
    int ret = rs_reconstruct(&rs, ds, validShards, shardSize);
    std::cout << "Reconstruction return code " << ret << std::endl;
    
    for (auto n : shardsToTest)
    {
        // Only data shards are recovered. Parity shards are not recovered, so don't display them.
        if (n < dataShards)
        {
            std::cout << "Recovered shard " << n << " data: ";
            for (int i = 0; i < shardSize; i++)
            {
                std::cout << std::setfill('0') << std::setw(2) << std::hex << (int) ds[n][i] << " ";
            }
            std::cout << std::endl;
        }
    }
}
