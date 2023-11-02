#include <stdio.h>
#include <string.h>
#include "appArgs.h"
#include "out.h"
#include "hash.h"

int main(int argc, char **argv)
{

    struct AppArgs appArgs;
    if(!doArgp(&appArgs, argc, argv)) return 2;

    if(!appArgs.value)
    {
        fprintf(stderr, "No value given to hash.\n");
        return 2;
    }

    uint8_t hash[16];

    size_t valueLen = strlen(appArgs.value);

    size_t hashSize = getHash(
        &appArgs,
        (uint8_t*)appArgs.value,
        valueLen,
        hash,
        sizeof hash
    );

    if(!hashSize) return 1;

    FILE *outF;
    if(appArgs.out[0] == '-' && appArgs.out[1] == '\0') outF = stdout;
    else outF = fopen(appArgs.out, "w");

    if(!outF)
    {
        fprintf(stderr, "Couldn't open file '%s' for writing.\n", appArgs.out);
        return 2;
    }

    printOut(hash, outF, &appArgs);

    fclose(outF);

    return 0;
}