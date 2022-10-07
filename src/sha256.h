#ifndef SHA256_H
#define SHA256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

// Number of bytes that are stored in a sha256 block
#define SHA256_BLOCK_SIZE (32)                 

typedef struct 
{
	uint8_t data[64];
	uint32_t datalen;
	uint64_t bitlen;
	uint32_t state[8];
} sha256_context_t;

/// @brief Stores the result of the hashing in a specified pointer
/// @param context The context that holds the hashed values
/// @param hash The pointer where the hash is stored
void sha256_final(sha256_context_t * context, uint8_t * hash);

/// @brief Initializes a sha256 context
/// @param context The context that is initialized
void sha256_init(sha256_context_t * context);

/// @brief Encrypts some data using sha-256
/// @param context The context where the result is stored
/// @param data The data that is encrypted
/// @param len The length of the data that is encrypted
void sha256_update(sha256_context_t * context, uint8_t const * data, size_t len);

#ifdef __cplusplus
}
#endif

#endif