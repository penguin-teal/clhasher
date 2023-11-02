#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdint.h>
#include "appArgs.h"

size_t getHash(struct AppArgs *args, uint8_t *in, size_t size, uint8_t *outBuf, size_t bufSize);

#endif