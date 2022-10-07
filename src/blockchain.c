#include "blockchain.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>
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
    genesisBlock.data = "Sic Mundus Creatus Est!";
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
    for (size_t i = 0; i < blockChain->difficulty / 2; i++)
        assert(!newBlock->ownHash[i]);
    if(blockChain->difficulty % 2)
        assert(!(newBlock->ownHash[blockChain->difficulty / 2] & 0xf0));
    *(blockChain->blocks + blockChain->usedBlocks) = *newBlock;
    blockChain->usedBlocks++;
    assert(blockChain->allocatedBlocks >= blockChain->usedBlocks);
    return 0;
}

void blockchain_print_blocks(block_chain_t blockChain)
{
    block_t * upperBlockBound = blockChain.blocks + blockChain.usedBlocks;
    for (block_t * blockPointer = blockChain.blocks; blockPointer < upperBlockBound; blockPointer++)
    {
        printf("=========================================================================================================\n");
        printf("BLOCK-%i\n", blockPointer->blockIndex);
        printf("Data:\t\t%s\n", blockPointer->data);
        if(blockPointer->previousHash)
        {
            printf("Previous hash:\t0x");
            uint8_t * prevoiusHashUpperBound = blockPointer->previousHash + SHA256_BLOCK_SIZE;
            for (uint8_t * php = blockPointer->previousHash; php < prevoiusHashUpperBound; php++)
                printf("%02x", *php);
            printf("\n");
        }
        else
            printf("Previous hash: NULL\n");
        printf("Own hash:\t0x");
        uint8_t * ownHashUpperBound = blockPointer->ownHash + SHA256_BLOCK_SIZE;
        for (uint8_t * ohp = blockPointer->ownHash; ohp < ownHashUpperBound; ohp++)
            printf("%02x", *ohp);
        printf("\n");       
    }    
}

void blockchain_free(block_chain_t blockChain)
{
    if(blockChain.blocks)
    {
        block_t * blockUpperBound = blockChain.blocks + blockChain.usedBlocks;
        for(block_t * blockPointer = blockChain.blocks + 1; blockPointer < blockUpperBound; blockPointer++)
            block_free(*blockPointer);
        free(blockChain.blocks);
        blockChain.blocks = NULL;
    }
}

static block_t blockchain_get_last_block(block_chain_t blockChain)
{
    return *(blockChain.blocks + blockChain.usedBlocks - 1);
}

static int blockchain_grow_block(block_chain_t * blockChain)
{
    assert(blockChain->allocatedBlocks >= blockChain->usedBlocks);
    block_t * newBlocks = malloc(sizeof(block_t) * blockChain->allocatedBlocks * BLOCK_GROWTH_FACTOR);
    if(!newBlocks)
        return -1;
    memcpy(newBlocks, blockChain->blocks, blockChain->allocatedBlocks * sizeof(block_t));
    blockChain->blocks = newBlocks;
    blockChain->allocatedBlocks *= BLOCK_GROWTH_FACTOR;
    assert(blockChain->allocatedBlocks > blockChain->usedBlocks);
    return 0;
}