#ifndef SHA256_H
#define SHA256_H

#include <stddef.h>

#define SHA256_BLOCK_SIZE (32)                 

typedef struct 
{
	unsigned char data[64];
	unsigned int datalen;
	unsigned long long bitlen;
	unsigned int state[8];
} sha256_context_t;

void sha256_init(sha256_context_t * context);
void sha256_update(sha256_context_t * context, unsigned char const * data, size_t len);
void sha256_final(sha256_context_t * context, unsigned char * hash);

#endif