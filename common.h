//
// Created by t123yh on 18-12-31.
//

#ifndef CNSS_TEMP_MEM_H
#define CNSS_TEMP_MEM_H

// No longer than BITS_PER_LONG; higher value means higher stack consumption and larger inversion tree
#define MAX_MATRIX_ORDER 24
#define MAX_MATRIX_SIZE (MAX_MATRIX_ORDER * MAX_MATRIX_ORDER)

#ifdef __KERNEL__

#include <linux/string.h>
#define MEM_ALLOC_SMALL(__SIZE__) ((unsigned char*)kmalloc((__SIZE__), GFP_KERNEL))
#define MEM_FREE_SMALL(__PTR__) (kfree((void*)(__PTR__)))
#include <linux/types.h>

#else

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define MEM_ALLOC_SMALL(__SIZE__) (malloc(__SIZE__))
#define MEM_FREE_SMALL(__PTR__) (free((void*)(__PTR__)))

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#define BIT(nr)            (1UL << (nr))
#define BITS_PER_LONG __WORDSIZE
#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)

#define BIT_ULL(nr)        (1ULL << (nr))
#define BIT_ULL_MASK(nr)    (1ULL << ((nr) % BITS_PER_LONG_LONG))
#define BIT_ULL_WORD(nr)    ((nr) / BITS_PER_LONG_LONG)
#define BITS_PER_BYTE        8

typedef unsigned gfp_t;

static inline void __set_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
    
    *p  |= mask;
}

static inline void __clear_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
    
    *p &= ~mask;
}

static inline int test_bit(int nr, const volatile unsigned long *addr)
{
    return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}


typedef unsigned gfp_t;

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif


#endif //CNSS_TEMP_MEM_H
