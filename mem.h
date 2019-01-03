//
// Created by t123yh on 18-12-31.
//

#ifndef CNSS_TEMP_MEM_H
#define CNSS_TEMP_MEM_H

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
#define MEM_ALLOC_SMALL(__SIZE__) ((uint8_t*)malloc(__SIZE__))
#define MEM_FREE_SMALL(__PTR__) (free((void*)(__PTR__)))

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#endif


#endif //CNSS_TEMP_MEM_H
