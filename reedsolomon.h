//
// Created by t123yh on 18-12-31.
//

#ifndef _WG_REEDSOLOMON_H
#define _WG_REEDSOLOMON_H

#include "common.h"
#include "radix-tree.h"

#ifdef __cplusplus
extern "C" {
#endif

struct reed_solomon
{
    uint8_t data_shard_count;
    uint8_t parity_shard_count;
    uint8_t base_matrix[MAX_MATRIX_SIZE];
    struct radix_tree_root *inversion_tree;
};

static inline uint8_t total_shards(const struct reed_solomon *rs)
{
    return rs->data_shard_count + rs->parity_shard_count;
}

int rs_init(struct reed_solomon *rs, uint8_t data_shard, uint8_t parity_shard);
void rs_encode(const struct reed_solomon *rs, uint8_t * const * shards, int shard_size);
int rs_reconstruct(const struct reed_solomon *rs, uint8_t * const * shards, unsigned long valid_shards, int shard_size);

#ifdef __cplusplus
}
#endif


#endif //_WG_REEDSOLOMON_H
