#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
#include <stdarg.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32_t blockIndex;
    uint32_t blockNonce;
    size_t dataAllocatedSize;
    size_t dataUsedSize;
    uint8_t * data;
    time_t blockTime;
    uint8_t * ownHash;
    uint8_t * previousHash;
}block_t;

void block_free(block_t block);

void block_free_multiple(uint32_t n, ...);

int block_init(block_t * block, char const * data);

void block_mine(block_t * block, uint32_t difficulty);

#ifdef __cplusplus
}
#endif

#endif