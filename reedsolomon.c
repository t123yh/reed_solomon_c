//
// Created by t123yh on 18-12-31.
//
#include <asm/errno.h>
#include <limits.h>
#include "common.h"
#include "reedsolomon.h"
#include "matrixoperations.h"
#include "galois.h"

int rs_init(struct reed_solomon *rs, uint8_t data_shard, uint8_t parity_shard)
{
    uint8_t r1[MAX_MATRIX_SIZE], r2[MAX_MATRIX_SIZE];
    
    radix_tree_init();
    
    if (data_shard + parity_shard > MAX_MATRIX_ORDER)
        return -EINVAL;
    
    rs->inversion_tree = MEM_ALLOC_SMALL(sizeof(struct radix_tree_root));
    if (rs->inversion_tree == NULL)
        return -ENOMEM;
    
    rs->data_shard_count = data_shard;
    rs->parity_shard_count = parity_shard;
    INIT_RADIX_TREE(rs->inversion_tree, GFP_KERNEL);
    
    rs_init_vandermonde(r2, data_shard + parity_shard, data_shard);
    rs_invert_square_matrix(r2, r1, data_shard);
    rs_matrix_multiply(r2, r1, rs->base_matrix, data_shard + parity_shard, data_shard, data_shard);
    
    return 0;
}

void rs_encode(const struct reed_solomon *rs, uint8_t * const * shards, int shard_size)
{
    uint8_t c, r;
    uint8_t * const * parity_shards = shards + rs->data_shard_count;
    
    for (c = 0; c < rs->parity_shard_count; c++)
    {
        memset(parity_shards[c], 0, shard_size);
    }
    
    for (c = 0; c < rs->data_shard_count; c++)
    {
        const uint8_t *rowIn = shards[c];
        for (r = 0; r < rs->parity_shard_count; r++)
        {
            uint8_t parity = rs->data_shard_count + r;
            rs_gal_mul_slice_xor(rs->base_matrix[parity * rs->data_shard_count + c], rowIn, parity_shards[r], shard_size);
        }
    }
}

int rs_reconstruct(const struct reed_solomon *rs, uint8_t * const * shards, unsigned long valid_shards, int shard_size)
{
    uint8_t i, j, k;
    int ret;
    
    // unsigned long missing_shards = ~existing_shards & ((1 << total_shards(rs)) - 1);
    valid_shards &= ((1 << total_shards(rs)) - 1);
    
    uint8_t * inversion = (uint8_t *)radix_tree_lookup(rs->inversion_tree, valid_shards);
    if (inversion == NULL)
    {
        uint8_t originalMatrix[MAX_MATRIX_SIZE];
        
        for (i = 0, j = 0; i < total_shards(rs) && j < rs->data_shard_count; i++)
        {
            if (test_bit(i, &valid_shards))
            {
                memcpy(&originalMatrix[j * rs->data_shard_count], &rs->base_matrix[i * rs->data_shard_count], rs->data_shard_count);
                j++;
            }
        }
        
        if (j < rs->data_shard_count)
            return -EINVAL;
        
        inversion = MEM_ALLOC_SMALL(rs->data_shard_count * rs->data_shard_count);
        if (inversion == NULL)
            return -ENOMEM;
        
        rs_invert_square_matrix(originalMatrix, inversion, rs->data_shard_count);
        ret = radix_tree_insert(rs->inversion_tree, valid_shards, inversion);
        if (ret != 0)
        {
            MEM_FREE_SMALL(inversion);
            if (ret == -EEXIST)
            {
                inversion = (uint8_t *) radix_tree_lookup(rs->inversion_tree, valid_shards);
            }
            else
            {
                return ret;
            }
        }
    }
    
    for (i = 0; i < rs->data_shard_count; i++)
    {
        if (!test_bit(i, &valid_shards))
        {
            memset(shards[i], 0, shard_size);
            for (j = 0, k = 0; j < rs->data_shard_count && k < total_shards(rs); k++)
            {
                if (test_bit(k, &valid_shards))
                {
                    rs_gal_mul_slice_xor(inversion[i * rs->data_shard_count + j], shards[k], shards[i], shard_size);
                    j++;
                }
            }
        }
    }
    
    return 0;
}

void rs_destroy(struct reed_solomon* rs)
{
    void **slot;
    struct radix_tree_iter iter;
    unsigned long last_index = ULONG_MAX;
    
    radix_tree_for_each_slot(slot, rs->inversion_tree, &iter, 0)
    {
        MEM_FREE_SMALL(*slot);
        if (last_index != ULONG_MAX)
            radix_tree_delete(rs->inversion_tree, last_index);
        last_index = iter.index;
    }
    if (last_index != ULONG_MAX)
        radix_tree_delete(rs->inversion_tree, last_index);
    
    MEM_FREE_SMALL(rs->inversion_tree);
    memset(rs, 0, sizeof(struct reed_solomon));
}