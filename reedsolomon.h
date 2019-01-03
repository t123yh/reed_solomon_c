//
// Created by t123yh on 18-12-31.
//

#ifndef _WG_REEDSOLOMON_H
#define _WG_REEDSOLOMON_H

#include "matrixoperations.h"
#include "matrixtree.h"

#ifdef __cplusplus
extern "C" {
#endif

struct reed_solomon
{
    unsigned char data_shard_count;
    unsigned char parity_shard_count;
    unsigned char base_matrix[MAX_MATRIX_SIZE];
    struct matrix_tree invert_matrix;
};

int rs_init(struct reed_solomon *rs, unsigned char data_shard, unsigned char parity_shard);
void rs_encode(const struct reed_solomon *rs, unsigned char * const * shards, int shard_size);

#ifdef __cplusplus
}
#endif


#endif //_WG_REEDSOLOMON_H
