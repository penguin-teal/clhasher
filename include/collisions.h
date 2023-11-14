#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <stdio.h>
#include "appArgs.h"
#include "list.h"

bool reportCollisions(struct AppArgs *appArgs, uint64_t hashListCount, size_t hashSize, uint8_t *hashList, FILE *outF, string_list_T *allInputs);

#endif

