#ifndef SHA256_H
#define SHA256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define SHA256_BLOCK_SIZE (32)                 

typedef struct 
{
	uint8_t data[64];
	uint32_t datalen;
	uint64_t bitlen;
	uint32_t state[8];
} sha256_context_t;

void sha256_init(sha256_context_t * context);
void sha256_update(sha256_context_t * context, uint8_t const * data, size_t len);
void sha256_final(sha256_context_t * context, uint8_t * hash);

#ifdef __cplusplus
}
#endif

#endif