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
        free(block.data);
    if(block.ownHash)
        free(block.ownHash);
}

void block_free_multiple(uint32_t n, ...)
{
    va_list ptr;
    va_start(ptr, n);
    block_t  block;
    for (int i = 0; i < n; i++)
    {
        block = va_arg(ptr, block_t);
        block_free(block);
    }
    return;
}

int block_init(block_t * block, char const * data)
{
    size_t dataSize = strlen(data);
    block->data = malloc(dataSize + 1);
    if(!block->data)
        return -1;
    block->blockIndex = 0;
    block->blockTime = time(NULL);
    block->blockNonce = 0;
    strcpy(block->data, data);
    *(block->data + dataSize) = '\0';
    block->dataAllocatedSize = dataSize + 1;
    block->dataUsedSize = dataSize;
    block->previousHash = NULL;
    block->ownHash = block_calculate_hash(block);
    if(*block->ownHash == '\0')
        return -1;
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
    unsigned char * blockStringified = malloc(4096);
    if(!blockStringified)
        return "";
    if(block->previousHash)
        sprintf(blockStringified, "%i%s%ju%i", block->blockIndex, block->previousHash, block->blockTime, block->blockNonce, block->data);
    else
        sprintf(blockStringified, "%i%ju%i", block->blockIndex, block->blockTime, block->blockNonce, block->data);
    blockStringified = strcat(blockStringified, block->data);
    sha256_context_t ctx;
    unsigned char * buf = malloc(SHA256_BLOCK_SIZE * sizeof(unsigned char));
    if(!buf)
        return "";
    sha256_init(&ctx);
	sha256_update(&ctx, blockStringified, strlen(blockStringified));
	sha256_final(&ctx, buf);
    free(blockStringified);
    return buf;
}