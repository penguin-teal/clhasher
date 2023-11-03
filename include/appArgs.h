#ifndef APPARGS_H
#define APPARGS_H

#include <stdbool.h>
#include <stdint.h>
#include "algorithm.h"

struct AppArgs
{
    char *value;
    char *out;
    bool verbose;
    enum Algorithm algorithm;
    uint32_t bits;
    uint32_t splitBits;
    bool hiToLo;
    uint32_t radix;
    uint64_t len;
    bool hashNul;
    bool annotate;
};

bool doArgp(struct AppArgs *appArgs, int argc, char **argv);

#endif