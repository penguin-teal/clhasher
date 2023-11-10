#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "appArgs.h"
#include "out.h"
#include "hash.h"
#include "escape.h"
#include "input.h"

int main(int argc, char **argv)
{
    char *inVal = NULL;
    struct AppArgs appArgs;
    if(!doArgp(&appArgs, argc, argv)) return 2;

    int code = getInput(&appArgs, &inVal);
    if(code) return code;

    if(appArgs.escape)
    {
        char *esc = mallocEscStr(appArgs.value);
        if(!esc) return 2;

        if(inVal) free(inVal);
        inVal = esc;
        appArgs.value = esc;
    }

    if(!appArgs.algorithm)
    {
        if(inVal) free(inVal);

        fprintf(stderr, "No algorithm given. Run with --help to see algorithm flags.\n");
        return 2;
    }

    FILE *outF;
    if(!strncmp(appArgs.out, "-", 2))
    {
        outF = stdout;
        if(appArgs.verbose) printf("Outputting to STDOUT.\n");
    }
    else
    {
        outF = fopen(appArgs.out, "w");
        if(appArgs.verbose) printf("Outputting to %s.\n", appArgs.out);
    }

    if(!outF)
    {
        if(inVal) free(inVal);

        fprintf(stderr, "Couldn't open file '%s' for writing.\n", appArgs.out);
        return 2;
    }

    doHashes(&appArgs, outF);

    fclose(outF);

    return 0;
}
