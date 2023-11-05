#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "appArgs.h"
#include "out.h"
#include "hash.h"
#include "readF.h"
#include "escape.h"

int main(int argc, char **argv)
{
    char *inVal = NULL;
    struct AppArgs appArgs;
    if(!doArgp(&appArgs, argc, argv)) return 2;

    if(!appArgs.value)
    {
        if(appArgs.in)
        {
            if(!strncmp(appArgs.in, "-", 2) && !appArgs.len)
            {
                inVal = mallocReadStdin(stdin);
                if(!inVal) return 2;
            }
            else
            {
                FILE *fIn;
                if(!strncmp(appArgs.in, "-", 2)) fIn = stdin;
                else fIn = fopen(appArgs.in, "rb");
                
                if(!fIn)
                {
                    fprintf(stderr, "Couldn't open input file '%s'.\n", appArgs.in);
                    return 2;
                }

                inVal = mallocFileBuffer(fIn, appArgs.len);

                fclose(fIn);

                if(!inVal) return 2;
            }

            appArgs.value = inVal;
        }
        else
        {
            fprintf(stderr, "No value given to hash. Run with --usage to see usage.\n");
            return 2;
        }
    }

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

    uint8_t hash[16];

    size_t valueLen;
    if(appArgs.len)
    {
        valueLen = appArgs.len;
    }
    else
    {
        valueLen = strlen(appArgs.value);
        if(appArgs.hashNul) valueLen++;
    }

    size_t hashSize = getHash(
        &appArgs,
        (uint8_t*)appArgs.value,
        valueLen,
        hash,
        sizeof hash
    );

    if(!hashSize)
    {
        if(inVal) free(inVal);

        fprintf(stderr, "Hashing failed. (got hashSize was zero)\n");
        return 1;
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

    printOut(hash, outF, &appArgs);

    fclose(outF);

    return 0;
}