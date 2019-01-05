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
    unsigned long *p = ((unsigned long *) addr) + BIT_WORD(nr);
    
    *p |= mask;
}

static inline void __clear_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *) addr) + BIT_WORD(nr);
    
    *p &= ~mask;
}

static inline int test_bit(int nr, const volatile unsigned long *addr)
{
    return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG - 1)));
}


typedef unsigned gfp_t;

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/**
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static __always_inline unsigned long __ffs(unsigned long word)
{
    int num = 0;

#if BITS_PER_LONG == 64
    if ((word & 0xffffffff) == 0)
    {
        num += 32;
        word >>= 32;
    }
#endif
    if ((word & 0xffff) == 0)
    {
        num += 16;
        word >>= 16;
    }
    if ((word & 0xff) == 0)
    {
        num += 8;
        word >>= 8;
    }
    if ((word & 0xf) == 0)
    {
        num += 4;
        word >>= 4;
    }
    if ((word & 0x3) == 0)
    {
        num += 2;
        word >>= 2;
    }
    if ((word & 0x1) == 0)
        num += 1;
    return num;
}

struct list_head {
    struct list_head *next, *prev;
};

#define __ARG_PLACEHOLDER_1 0,
#define __take_second_arg(__ignored, val, ...) val

/*
 * The use of "&&" / "||" is limited in certain expressions.
 * The following enable to calculate "and" / "or" with macro expansion only.
 */
#define __and(x, y)			___and(x, y)
#define ___and(x, y)			____and(__ARG_PLACEHOLDER_##x, y)
#define ____and(arg1_or_junk, y)	__take_second_arg(arg1_or_junk y, 0)

#define __or(x, y)			___or(x, y)
#define ___or(x, y)			____or(__ARG_PLACEHOLDER_##x, y)
#define ____or(arg1_or_junk, y)		__take_second_arg(arg1_or_junk 1, y)

/*
 * Helper macros to use CONFIG_ options in C/CPP expressions. Note that
 * these only work with boolean and tristate options.
 */

/*
 * Getting something that works in C and CPP for an arg that may or may
 * not be defined is tricky.  Here, if we have "#define CONFIG_BOOGER 1"
 * we match on the placeholder define, insert the "0," for arg1 and generate
 * the triplet (0, 1, 0).  Then the last step cherry picks the 2nd arg (a one).
 * When CONFIG_BOOGER is not defined, we generate a (... 1, 0) pair, and when
 * the last step cherry picks the 2nd arg, we get a zero.
 */
#define __is_defined(x)			___is_defined(x)
#define ___is_defined(val)		____is_defined(__ARG_PLACEHOLDER_##val)
#define ____is_defined(arg1_or_junk)	__take_second_arg(arg1_or_junk 1, 0)

/*
 * IS_BUILTIN(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'y', 0
 * otherwise. For boolean options, this is equivalent to
 * IS_ENABLED(CONFIG_FOO).
 */
#define IS_BUILTIN(option) __is_defined(option)

/*
 * IS_MODULE(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'm', 0
 * otherwise.
 */
#define IS_MODULE(option) __is_defined(option##_MODULE)

/*
 * IS_REACHABLE(CONFIG_FOO) evaluates to 1 if the currently compiled
 * code can call a function defined in code compiled based on CONFIG_FOO.
 * This is similar to IS_ENABLED(), but returns false when invoked from
 * built-in code when CONFIG_FOO is set to 'm'.
 */
#define IS_REACHABLE(option) __or(IS_BUILTIN(option), \
				__and(IS_MODULE(option), __is_defined(MODULE)))

/*
 * IS_ENABLED(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'y' or 'm',
 * 0 otherwise.
 */
#define IS_ENABLED(option) __or(IS_BUILTIN(option), IS_MODULE(option))

#include <stdio.h>
#define BUG_ON(PREDICATE) ({if (PREDICATE) {printf("BUG!!\n"); }})
#define WARN_ON(PREDICATE) ({if (PREDICATE) {printf("WARN!!\n"); }})
#define WARN_ON_ONCE(PREDICATE) ({if (PREDICATE) {printf("WARN!!\n"); }})
#define __force
#define __rcu
#define __read_mostly
struct rcu_head {};
inline static void *rcu_dereference(void *pVoid) { return pVoid; }
inline static void *rcu_dereference_raw(void *pVoid) { return pVoid; }
#define __must_check __attribute__((warn_unused_result))
#define rcu_assign_pointer(p, v) ((p) = (v))

#define READ_ONCE(x) (x)
#define WRITE_ONCE(x, val) (x = val)

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const struct list_head *head)
{
    return READ_ONCE(head->next) == head;
}

#define __GFP_BITS_SHIFT (23 + IS_ENABLED(CONFIG_LOCKDEP))
#define __GFP_BITS_MASK ((__force gfp_t)((1 << __GFP_BITS_SHIFT) - 1))
#define CONFIG_BASE_SMALL 0
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
/*
 * In case of changes, please don't forget to update
 * include/trace/events/mmflags.h and tools/perf/builtin-kmem.c
 */

/* Plain integer GFP bitmasks. Do not use this directly. */
#define ___GFP_DMA		0x01u
#define ___GFP_HIGHMEM		0x02u
#define ___GFP_DMA32		0x04u
#define ___GFP_MOVABLE		0x08u
#define ___GFP_RECLAIMABLE	0x10u
#define ___GFP_HIGH		0x20u
#define ___GFP_IO		0x40u
#define ___GFP_FS		0x80u
#define ___GFP_WRITE		0x100u
#define ___GFP_NOWARN		0x200u
#define ___GFP_RETRY_MAYFAIL	0x400u
#define ___GFP_NOFAIL		0x800u
#define ___GFP_NORETRY		0x1000u
#define ___GFP_MEMALLOC		0x2000u
#define ___GFP_COMP		0x4000u
#define ___GFP_ZERO		0x8000u
#define ___GFP_NOMEMALLOC	0x10000u
#define ___GFP_HARDWALL		0x20000u
#define ___GFP_THISNODE		0x40000u
#define ___GFP_ATOMIC		0x80000u
#define ___GFP_ACCOUNT		0x100000u
#define ___GFP_DIRECT_RECLAIM	0x200000u
#define ___GFP_KSWAPD_RECLAIM	0x400000u
#ifdef CONFIG_LOCKDEP
#define ___GFP_NOLOCKDEP	0x800000u
#else
#define ___GFP_NOLOCKDEP	0
#endif
/* If the above are modified, __GFP_BITS_SHIFT may need updating */

/*
 * Physical address zone modifiers (see linux/mmzone.h - low four bits)
 *
 * Do not put any conditional on these. If necessary modify the definitions
 * without the underscores and use them consistently. The definitions here may
 * be used in bit comparisons.
 */
#define __GFP_DMA	((__force gfp_t)___GFP_DMA)
#define __GFP_HIGHMEM	((__force gfp_t)___GFP_HIGHMEM)
#define __GFP_DMA32	((__force gfp_t)___GFP_DMA32)
#define __GFP_MOVABLE	((__force gfp_t)___GFP_MOVABLE)  /* ZONE_MOVABLE allowed */
#define GFP_ZONEMASK	(__GFP_DMA|__GFP_HIGHMEM|__GFP_DMA32|__GFP_MOVABLE)


#define u32 unsigned int
#define u64 unsigned long long

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    WRITE_ONCE(list->next, list);
    list->prev = list;
}

#define BITS_PER_TYPE(type) (sizeof(type) * BITS_PER_BYTE)
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_TYPE(long))

#endif


#endif //CNSS_TEMP_MEM_H
