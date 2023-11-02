#include <stddef.h>
#include "ints.h"

#define SEED_32  0x811C9DC5
#define PRIME_32 0x01000193

#define SEED_64  0xCBF29CE484222325
#define PRIME_64 0x100000001B3

#define SEED_128_HI  0x6C62272E07BB0142
#define PRIME_128_HI 0x0000000001000000
#define SEED_128_LO  0x62B821756295C58D 
#define PRIME_128_LO 0x000000000000013B

uint32_t fnv1a_32(uint8_t *buf, size_t n)
{
    uint32_t hash = SEED_32;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash ^ buf[i]) * PRIME_32;
    }

    return hash;
}

uint64_t fnv1a_64(uint8_t *buf, size_t n)
{
    uint64_t hash = SEED_64;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash ^ buf[i]) * PRIME_64;
    }

    return hash;
}

u128 fnv1a_128(uint8_t *buf, size_t n)
{
    u128 hash = ((u128)SEED_128_HI << 64) | SEED_128_LO;
    u128 prime = ((u128)PRIME_128_HI << 64) | PRIME_128_LO;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash ^ buf[i]) * prime;
    }

    return hash;
}

uint32_t fnv1_32(uint8_t *buf, size_t n)
{
    uint32_t hash = SEED_32;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash * PRIME_32) ^ buf[i];
    }

    return hash;
}

uint64_t fnv1_64(uint8_t *buf, size_t n)
{
    uint64_t hash = SEED_64;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash * PRIME_64) ^ buf[i];
    }

    return hash;
}

u128 fnv1_128(uint8_t *buf, size_t n)
{
    u128 hash = ((u128)SEED_128_HI << 64) | SEED_128_LO;
    u128 prime = ((u128)PRIME_128_HI << 64) | PRIME_128_LO;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash * prime) ^ buf[i];
    }

    return hash;
}

uint32_t fnv0_32(uint8_t *buf, size_t n)
{
    uint32_t hash = 0;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash * PRIME_32) ^ buf[i];
    }

    return hash;
}

uint64_t fnv0_64(uint8_t *buf, size_t n)
{
    uint64_t hash = 0;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash * PRIME_64) ^ buf[i];
    }

    return hash;
}

u128 fnv0_128(uint8_t *buf, size_t n)
{
    u128 hash = 0;
    u128 prime = ((u128)PRIME_128_HI << 64) | PRIME_128_LO;
    for(uint64_t i = 0; i < n; i++)
    {
        hash = (hash * prime) ^ buf[i];
    }

    return hash;
}


