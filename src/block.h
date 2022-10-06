#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
#include <stdarg.h>
#include <time.h>

typedef struct
{
    uint32_t blockIndex;
    uint32_t blockNonce;
    size_t dataAllocatedSize;
    size_t dataUsedSize;
    unsigned char * data;
    time_t blockTime;
    unsigned char * ownHash;
    unsigned char * previousHash;
}block_t;

void block_free(block_t block);

void block_free_multiple(uint32_t n, ...);

int block_init(block_t * block, char const * data);

void block_mine(block_t * block, uint32_t difficulty);

#endif