#include <gtest/gtest.h>

#include <string.h>

#include "sha256.h"

TEST(SHA256, abc)
{
    uint8_t expectedHash[SHA256_BLOCK_SIZE] =  {
                                            0xba,
                                            0x78,
                                            0x16,
                                            0xbf,
                                            0x8f,
                                            0x01,
                                            0xcf,
                                            0xea,
                                            0x41,
                                            0x41,
                                            0x40,
                                            0xde,
                                            0x5d,
                                            0xae,
                                            0x22,
                                            0x23, 
                                            0xb0,
                                            0x03,
                                            0x61,
                                            0xa3,
                                            0x96,
                                            0x17,
                                            0x7a,
                                            0x9c,
                                            0xb4,
                                            0x10,
                                            0xff,
                                            0x61,
                                            0xf2,
                                            0x00,
                                            0x15,
                                            0xad
                                        };
    uint8_t text[] = {"abc"};
    uint8_t buf[SHA256_BLOCK_SIZE];
    sha256_context_t ctx;
    sha256_init(&ctx);
	sha256_update(&ctx, text, strlen((char *)text));
	sha256_final(&ctx, buf);
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++)
        ASSERT_EQ(*(buf + i), *(expectedHash + i));
}

TEST(SHA256, enigma)
{
    uint8_t expectedHash[SHA256_BLOCK_SIZE] =  {
                                            0x67,
                                            0xa4,
                                            0xf4,
                                            0x5f,
                                            0x0d,
                                            0x1d,
                                            0x9b,
                                            0xc6,
                                            0x06,
                                            0x48,
                                            0x6f,
                                            0xc4,
                                            0x2d,
                                            0xc4,
                                            0x94,
                                            0x16, 
                                            0x68, 
                                            0xe7, 
                                            0x1d, 
                                            0x34, 
                                            0xee, 
                                            0x50, 
                                            0x07, 
                                            0x35, 
                                            0xfe, 
                                            0x9B, 
                                            0x7e, 
                                            0xa4, 
                                            0x62, 
                                            0x5c, 
                                            0x68, 
                                            0x7c
                                        };
    uint8_t text[] = {"enigma"};
    uint8_t buf[SHA256_BLOCK_SIZE];
    sha256_context_t ctx;
    sha256_init(&ctx);
	sha256_update(&ctx, text, strlen((char *)text));
	sha256_final(&ctx, buf);
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++)
        ASSERT_EQ(*(buf + i), *(expectedHash + i));
}