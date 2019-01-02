//
// Created by t123yh on 18-12-31.
//
#include <asm/errno.h>
#include "mem.h"
#include "reedsolomon.h"
#include "matrixoperations.h"
#include "galois.h"

int rs_init(struct reed_solomon *rs, unsigned char data_shard, unsigned char parity_shard)
{
    unsigned char *r1, *r2;
    int ret = 0;
    
    r1 = MEM_ALLOC_SMALL(data_shard * data_shard);
    r2 = MEM_ALLOC_SMALL((data_shard + parity_shard) * data_shard);
    rs->base_matrix = MEM_ALLOC_SMALL((data_shard + parity_shard) * data_shard);
    if (unlikely(!r1 || !r2 || !rs->base_matrix))
    {
        ret = -ENOMEM;
        goto out;
    }
    
    rs->data_shard_count = data_shard;
    rs->parity_shard_count = parity_shard;
    rs_init_vandermonde(r2, data_shard + parity_shard, data_shard);
    ret = rs_invert_square_matrix(r2, r1, data_shard);
    if (unlikely(ret != 0))
        goto out;
    rs_matrix_multiply(r2, r1, rs->base_matrix, data_shard + parity_shard, data_shard, data_shard);
    
    out:
    MEM_FREE_SMALL(r2);
    MEM_FREE_SMALL(r1);
    if (ret != 0)
        MEM_FREE_SMALL(rs->base_matrix);
    return ret;
}

void rs_encode(const struct reed_solomon *rs, const unsigned char * const * data_shards, unsigned char * const * parity_shards, int shard_size)
{
    unsigned char c, r;
    
    for (c = 0; c < rs->parity_shard_count; c++)
    {
        memset(parity_shards[c], 0, shard_size);
    }
    
    for (c = 0; c < rs->data_shard_count; c++)
    {
        const unsigned char *rowIn = data_shards[c];
        for (r = 0; r < rs->parity_shard_count; r++)
        {
            unsigned char parity = rs->data_shard_count + r;
            rs_gal_mul_slice_xor(rs->base_matrix[parity * rs->data_shard_count + c], rowIn, parity_shards[r], shard_size);
        }
    }
}

