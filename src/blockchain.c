#include "blockchain.h"

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include "sha256.h"

#define BLOCK_INIT_SIZE (2)

#define BLOCK_GROWTH_FACTOR (2)

static block_t blockchain_get_last_block();
static int blockchain_grow_block();

int blockchain_init(block_chain_t * blockChain, uint32_t difficulty)
{
    blockChain->blocks = malloc(sizeof(block_t) * BLOCK_INIT_SIZE);
    if(!blockChain->blocks)
        return -1;
    blockChain->allocatedBlocks = BLOCK_INIT_SIZE;
    blockChain->usedBlocks = 1;
    blockChain->difficulty = difficulty;
    block_t genesisBlock;
    genesisBlock.previousHash = NULL;
    genesisBlock.data = "It shall be light!";
    genesisBlock.blockIndex = 0;
    *blockChain->blocks = genesisBlock;
    return 0;
}

int blockchain_add_block(block_chain_t * blockChain, block_t * newBlock)
{
    newBlock->previousHash = blockchain_get_last_block(*blockChain).ownHash;
    newBlock->blockIndex = blockchain_get_last_block(*blockChain).blockIndex + 1;
    if(blockChain->usedBlocks == blockChain->allocatedBlocks)
        if(blockchain_grow_block(blockChain))
            return -1;
    block_mine(newBlock, blockChain->difficulty);
    *(blockChain->blocks + blockChain->usedBlocks) = *newBlock;
    blockChain->usedBlocks++;
    return 0;
}

void blockchain_print_blocks(block_chain_t blockChain)
{
    for (size_t i = 0; i < blockChain.usedBlocks; i++)
    {
        printf("Index: %i\n", (blockChain.blocks + i)->blockIndex);
        printf("Data: %s\n", (blockChain.blocks + i)->data);
        printf("Own hash: 0x");
        for (size_t j = 0; j < SHA256_BLOCK_SIZE; j++)
            printf("%02x", *((blockChain.blocks + i)->ownHash + j));
        printf("\n");
        if((blockChain.blocks + i)->previousHash)
        {
            printf("Previous hash: 0x");
            for (size_t k = 0; k < SHA256_BLOCK_SIZE; k++)
                printf("%02x", *((blockChain.blocks + i)->previousHash + k));
            printf("\n");
        }
        else
            printf("Previous hash: NULL\n");
    }    
}

void blockchain_free(block_chain_t blockChain)
{
    if(blockChain.blocks)
    {
        for(size_t i = 1; i < blockChain.usedBlocks; i++)
            block_free(*(blockChain.blocks + i));
        free(blockChain.blocks);
    }
}

static block_t blockchain_get_last_block(block_chain_t blockChain)
{
    return *(blockChain.blocks + blockChain.usedBlocks - 1);
}

static int blockchain_grow_block(block_chain_t * blockChain)
{
    block_t * newBlocks = malloc(sizeof(block_t) * blockChain->allocatedBlocks * BLOCK_GROWTH_FACTOR);
    if(!newBlocks)
        return -1;
    memcpy(newBlocks, blockChain->blocks, blockChain->allocatedBlocks * sizeof(block_t));
    blockChain->blocks = newBlocks;
    blockChain->allocatedBlocks *= BLOCK_GROWTH_FACTOR;
    return 0;
}