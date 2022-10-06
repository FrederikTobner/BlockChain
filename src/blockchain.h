#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stddef.h>

#include "block.h"

typedef struct 
{
    uint32_t difficulty;
    size_t allocatedBlocks;
    size_t usedBlocks;
    block_t * blocks;
}block_chain_t;

int blockchain_init(block_chain_t * blockChain, uint32_t difficulty);

int blockchain_add_block(block_chain_t * blockChain, block_t * newBlock);

void blockchain_print_blocks(block_chain_t blockChain);

void blockchain_free(block_chain_t blockChain);

#endif