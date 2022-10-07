#include "block.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sha256.h"

static char * block_calculate_hash();

void block_free(block_t block)
{
    if(block.data)
    {
        free(block.data);
        block.data = NULL;
    }
    if(block.ownHash)
    {
        free(block.ownHash);
        block.ownHash = NULL;
    }
}

int block_init(block_t * block, char const * data)
{
    size_t dataSize = strlen(data);
    block->data = malloc(dataSize + 1);
    if(!block->data)
        return -1; // Allocating the memory needed for the data failed 😨
    block->blockIndex = 0;
    block->blockTime = time(NULL);
    block->blockNonce = 0;
    strcpy(block->data, data);
    *(block->data + dataSize) = '\0';
    block->dataAllocatedSize = dataSize + 1;
    block->dataUsedSize = dataSize;
    block->previousHash = NULL;
    block->ownHash = block_calculate_hash(block);
    if(!block->ownHash)
        return -1; // Hashing the block failed 😨
    return 0;
}

void block_mine(block_t * block, uint32_t difficulty)
{ 
    uint32_t loopCount = difficulty / 2u;
    bool uneven = false;
    if(difficulty % 2 == 1)
    {
        loopCount++;
        uneven = true;
    }
    
    for(;;)
    {
        free(block->ownHash);
        block->blockNonce++;
        block->ownHash = block_calculate_hash(block);
       
        for (uint32_t i = 0; i < loopCount; i++)
        {
            if(i == (loopCount - 1u) && uneven)
            {
                if(*(block->ownHash + i) & 0xf0)
                    break;
                return;
            }
            else
            {
                if(*(block->ownHash + i) != 0)                
                    break;
                if(i == (loopCount - 1u))
                 return;
            }          

        }        
    }
}

static char * block_calculate_hash(block_t * block)
{
    uint8_t * blockStringified = malloc(4096 * sizeof(uint8_t));
    if(!blockStringified)
        return NULL;
    // Max length for block content without data 
    if(block->previousHash)
        sprintf(blockStringified, "%i%ju%i", block->blockIndex, block->blockTime, block->blockNonce);
    else
        sprintf(blockStringified, "%i%ju%i", block->blockIndex, block->blockTime, block->blockNonce);
    if(strlen(blockStringified) + strlen(block->data) + SHA256_BLOCK_SIZE * 2 >= 4096)
        return NULL;  // Buffer is not big enough to store all the data that the block contains
    if(block->previousHash)
    {
        char prevoiusHashStringified [SHA256_BLOCK_SIZE * 2];
        for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++)
            sprintf(prevoiusHashStringified, "%s%02x", prevoiusHashStringified, *(block->previousHash + i));
        blockStringified = strcat(blockStringified, prevoiusHashStringified);
    }
    blockStringified = strcat(blockStringified, block->data);
    sha256_context_t ctx;
    uint8_t * buf = malloc(SHA256_BLOCK_SIZE * sizeof(uint8_t));
    if(!buf)
        return NULL;
    sha256_init(&ctx);
	sha256_update(&ctx, blockStringified, strlen(blockStringified));
	sha256_final(&ctx, buf);
    free(blockStringified);
    return buf;
}