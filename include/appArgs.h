#ifndef APPARGS_H
#define APPARGS_H

#include <stdbool.h>
#include "algorithm.h"

struct AppArgs
{
    char *value;
    char *out;
    bool verbose;
    enum Algorithm algorithm;
};

bool doArgp(struct AppArgs *appArgs, int argc, char **argv);

#endif