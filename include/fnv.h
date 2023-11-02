#ifndef FNV_H
#define FNV_H

#include <stddef.h>
#include <ints.h>

uint32_t fnv1a_32(uint8_t *buf, size_t n);

uint64_t fnv1a_64(uint8_t *buf, size_t n);

u128 fnv1a_128(uint8_t *buf, size_t n);

uint32_t fnv1_32(uint8_t *buf, size_t n);

uint64_t fnv1_64(uint8_t *buf, size_t n);

u128 fnv1_128(uint8_t *buf, size_t n);

uint32_t fnv0_32(uint8_t *buf, size_t n);

uint64_t fnv0_64(uint8_t *buf, size_t n);

u128 fnv0_128(uint8_t *buf, size_t n);

#endif