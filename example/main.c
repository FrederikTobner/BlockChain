#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "blockchain.h"
#include "sha256.h"

int main(int argc, char const ** argv)
{    
    block_chain_t bc;
    if(blockchain_init(&bc, 4))
        return -1;                  // Blockchain creation failed
    block_t b1, b2, b3, b4;
    
    if(!block_init(&b1, "Foo"))
        blockchain_add_block(&bc, &b1);
    
    if(!block_init(&b2, "Bar"))
        blockchain_add_block(&bc, &b2);
    
    if(!block_init(&b3, "Baz"))
        blockchain_add_block(&bc, &b3);
    
    if(!block_init(&b4, "Qux"))
        blockchain_add_block(&bc, &b4);

    blockchain_print_blocks(bc);
    blockchain_free(bc);
    return 0;
}
