#include "block.h"

#include "sha256.h"

static char * block_calculate_hash();

void block_free(block_t block)
{
    if(block.data)
    {
        free((char *)block.data);
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
    char * dataBuffer = malloc(dataSize + 1);
    if(!dataBuffer)
        return -1; // Allocating the memory needed for the data failed 😨
    block->blockIndex = 0u;
    block->blockTime = time(NULL);
    block->blockNonce = 0u;
    strcpy(dataBuffer, data);
    *(dataBuffer + dataSize) = '\0';
    block->data = dataBuffer;
    block->dataAllocatedSize = dataSize + 1;
    block->dataUsedSize = dataSize;
    block->previousHash = NULL;
    uint8_t * merkleBuf = malloc(SHA256_BLOCK_SIZE * sizeof(uint8_t));
    if(!merkleBuf)
        return -1;
    sha256_context_t context;    
    sha256_init(&context);
	sha256_update(&context, data, dataSize);
	sha256_final(&context, merkleBuf);
    block->merkleRoot = merkleBuf;
    block->ownHash = block_calculate_hash(block);
    if(!block->ownHash)
        return -1; // Hashing the block failed 😨
    return 0;
}

void block_mine(block_t * block, uint32_t difficulty)
{ 
    uint32_t loopCount = difficulty / 2u;
    bool uneven = false;
    if(difficulty % 2)
    {
        loopCount++;
        uneven = true;
    }    
    for(;;)
    {        
        block->blockNonce++;
        block->ownHash = block_calculate_hash(block);       
        for (uint32_t i = 0; i < loopCount; i++)
        {
            if(i == loopCount - 1 && uneven)
            {
                if(*(block->ownHash + i) & 0xf0)
                    break;
                block->blockTime = time(NULL);
                return;
            }
            else
            {
                if(*(block->ownHash + i))                
                    break;
                if(i == loopCount - 1)
                {
                    block->blockTime = time(NULL);
                    return;
                }
            }          

        }
        free(block->ownHash);      
    }
}

void block_print(block_t block)
{
    printf("=========================================================================================================\n");
    printf("BLOCK-%zd\n", block.blockIndex);
    printf("Data:\t\t%s\n", block.data);
    if(block.previousHash)
    {
        printf("Previous hash:\t0x");
        uint8_t * prevoiusHashUpperBound = block.previousHash + SHA256_BLOCK_SIZE;
        for (uint8_t * previousHashPointer = block.previousHash; previousHashPointer < prevoiusHashUpperBound; previousHashPointer++)
            printf("%02x", *previousHashPointer);
        printf("\n");
    }
    printf("Own hash:\t0x");
    uint8_t * ownHashUpperBound = block.ownHash + SHA256_BLOCK_SIZE;
    for (uint8_t * ownHashPointer = block.ownHash; ownHashPointer < ownHashUpperBound; ownHashPointer++)
        printf("%02x", *ownHashPointer);
    printf("\n");
    printf("Merkle root:\t0x");
     uint8_t * merkleHashUpperBound = block.merkleRoot + SHA256_BLOCK_SIZE;
    for (uint8_t * merkleRootPointer = block.merkleRoot; merkleRootPointer < merkleHashUpperBound; merkleRootPointer++)
        printf("%02x", *merkleRootPointer);
    printf("\n"); 
}

static char * block_calculate_hash(block_t * block)
{
    uint8_t * blockStringified = malloc(4096 * sizeof(uint8_t));
    if(!blockStringified)
        return NULL;
    // Max length for block content without data is always below 4096 (9 (index) + 9 (nonce) + 17 (time))
    sprintf(blockStringified, "%zd%ju%i", block->blockIndex, block->blockTime, block->blockNonce);
    if(strlen(blockStringified) + strlen(block->data) + SHA256_BLOCK_SIZE * 2 >= 4096)
        return NULL;  // Buffer is not big enough to store all the data that the block contains
    if(block->previousHash)
    {
        char prevoiusHashStringified [SHA256_BLOCK_SIZE * 2];
        uint8_t * upperBound =  block->previousHash + SHA256_BLOCK_SIZE;
        for (uint8_t * blockPointer = block->previousHash; blockPointer < upperBound; blockPointer++)
            sprintf(prevoiusHashStringified, "%s%02x", prevoiusHashStringified, *blockPointer);
        blockStringified = strcat(blockStringified, prevoiusHashStringified);
    }
    blockStringified = strcat(blockStringified, block->merkleRoot);    
    uint8_t * buf = malloc(SHA256_BLOCK_SIZE * sizeof(uint8_t));
    if(!buf)
        return NULL;
    sha256_context_t context; 
    sha256_init(&context);
	sha256_update(&context, blockStringified, strlen(blockStringified));
	sha256_final(&context, buf);
    free(blockStringified);
    return buf;
}