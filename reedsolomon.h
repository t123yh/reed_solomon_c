//
// Created by t123yh on 18-12-31.
//

#ifndef _WG_REEDSOLOMON_H
#define _WG_REEDSOLOMON_H

#include "matrixtree.h"

#ifdef __cplusplus
extern "C" {
#endif

struct reed_solomon
{
    int data_shard_count;
    int parity_shard_count;
    unsigned char *base_matrix;
    struct matrix_tree invert_matrix;
};

int rs_init(struct reed_solomon *rs, unsigned char data_shard, unsigned char parity_shard);

#ifdef __cplusplus
}
#endif


#endif //_WG_REEDSOLOMON_H
