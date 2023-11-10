#ifndef OUT_H
#define OUT_H

#include <stdint.h>
#include <stdio.h>
#include "appArgs.h"

void printOut(uint8_t *buf, FILE *f, struct AppArgs *appArgs, uint64_t hashNumber);

#endif
