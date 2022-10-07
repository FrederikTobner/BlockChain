#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "block.h"

typedef struct 
{
    uint32_t difficulty;        // The difficulty of the mining process (amount of leading 0h halfbytes -> 0000, in the ownhash)
    size_t allocatedBlocks;     // The amount of allicated bytes of the dynamic "blocks" array
    size_t usedBlocks;          // The amount of used bytes in the dynamic "blocks" array
    block_t * blocks;           // The blocks that the blockchain contains (dynamic array)
}block_chain_t;

/// @brief Adds a new block to the blockchain
/// @param blockChain The blockchain where a new block is added
/// @param newBlock The block that is added
/// @return 0 if sucessful
int blockchain_add_block(block_chain_t * blockChain, block_t * newBlock);

/// @brief Frees the dynamic memory previously used in the the blockchain (blocks)
/// @param blockChain The blockchain where the contents are freed
void blockchain_free(block_chain_t blockChain);

/// @brief Initializes a blockchain with the specified difficulty
/// @param blockChain The blockchain that is initialized
/// @param difficulty The difficulty that the blockchain uses
/// @return 0 if sucessful
int blockchain_init(block_chain_t * blockChain, uint32_t difficulty);

/// @brief Prints the blocks that the blockchain contains
/// @param blockChain The blockchain that is printed
void blockchain_print_blocks(block_chain_t blockChain);

#ifdef __cplusplus
}
#endif

#endif