#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include "appArgs.h"

bool doHash(struct AppArgs *appArgs, FILE *outF, uint8_t *outHash, uint64_t hashNumber);

#endif
