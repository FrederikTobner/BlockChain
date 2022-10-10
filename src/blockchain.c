#include "blockchain.h"

#include "sha256.h"

#define BLOCK_INIT_SIZE (2)

#define BLOCK_GROWTH_FACTOR (2)

static inline block_t blockchain_get_last_block();
static int blockchain_grow_block();

int blockchain_init(block_chain_t * blockChain, uint8_t difficulty)
{
    blockChain->blocks = malloc(sizeof(block_t) * BLOCK_INIT_SIZE);
    if(!blockChain->blocks)
        return -1;
    blockChain->allocatedBlocks = BLOCK_INIT_SIZE;
    blockChain->usedBlocks = 1;
    blockChain->difficulty = difficulty;
    block_t genesisBlock;
    block_init(&genesisBlock, "Sic Mundus Creatus Est!");
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
    
    // Validate own hash
    uint8_t * upperBound = newBlock->ownHash + blockChain->difficulty / 2;
    for (uint8_t * ownHashPointer = newBlock->ownHash; ownHashPointer < upperBound; ownHashPointer++)
        assert(!*ownHashPointer);
    if(blockChain->difficulty % 2)
        assert(!(*(newBlock->ownHash + blockChain->difficulty / 2) & 0xf0));
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
        block_print(*blockPointer);      
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
        blockChain.usedBlocks = 0;
        blockChain.allocatedBlocks = 0;
    }
}

static inline block_t blockchain_get_last_block(block_chain_t blockChain)
{
    return *(blockChain.blocks + blockChain.usedBlocks - 1);
}

static int blockchain_grow_block(block_chain_t * blockChain)
{
    assert(blockChain->allocatedBlocks >= blockChain->usedBlocks);
    size_t newAllocatedCount = blockChain->allocatedBlocks != 0 ? blockChain->allocatedBlocks * BLOCK_GROWTH_FACTOR : BLOCK_INIT_SIZE;
    block_t * newBlocks = malloc(sizeof(block_t) * newAllocatedCount);
    if(!newBlocks)
        return -1;
    memcpy(newBlocks, blockChain->blocks, blockChain->allocatedBlocks * sizeof(block_t));
    blockChain->blocks = newBlocks;
    blockChain->allocatedBlocks  = newAllocatedCount;
    assert(blockChain->allocatedBlocks > blockChain->usedBlocks);
    return 0;
}