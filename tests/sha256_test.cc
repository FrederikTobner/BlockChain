#include <gtest/gtest.h>

#include <string.h>

extern "C"
{
#include "sha256.h"
}

TEST(SHA256, simpleHash)
{
    unsigned char hash1[SHA256_BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
	                                 0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
    unsigned char text1[] = {"abc"};
    unsigned char buf[SHA256_BLOCK_SIZE];
    sha256_context_t ctx;
    sha256_init(&ctx);
	sha256_update(&ctx, text1, strlen((char *)text1));
	sha256_final(&ctx, buf);
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++)
        ASSERT_EQ(*(buf + i), *(hash1 + i));
}