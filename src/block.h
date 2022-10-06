#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
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

/// @brief Frees the dynamic memory previously used in the the block (ownhash and data) 
/// @param block The block where the contents are freed
void block_free(block_t block);

/// @brief Initializes a block with the specified data
/// @param block The block that is initialized
/// @param data The data that is stored in the block
/// @return 0 if sucessful
int block_init(block_t * block, char const * data);

/// @brief Mines a block with the specified difficulty
/// @param block The block that is mined
/// @param difficulty The difficulty of the mining process
void block_mine(block_t * block, uint32_t difficulty);

#ifdef __cplusplus
}
#endif

#endif