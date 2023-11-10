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
    int ret;
    char *inVal = NULL;
    struct AppArgs appArgs;

    if(!doArgp(&appArgs, argc, argv)) return 2;

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
        fprintf(stderr, "Couldn't open file '%s' for writing.\n", appArgs.out);
        return 2;
    }

    uint64_t hashCount = 0;
    bool gotInput;
    do
    {
        gotInput = getInput(&appArgs, &inVal);
        if(!gotInput)
        {
            if(appArgs.multi) break;
            else
            {
                ret = 2;
                goto CloseFile;
            }
        }
        else hashCount++;

        if(appArgs.escape)
        {
            char *esc = mallocEscStr(appArgs.value);
            if(!esc)
            {
                ret = 2;
                goto CloseFile;
            }

            if(inVal) free(inVal);
            inVal = esc;
            appArgs.value = esc;
        }

        if(!appArgs.algorithm)
        {
            if(inVal) free(inVal);

            fprintf(stderr, "No algorithm given. Run with --help to see algorithm flags.\n");
            ret = 2;
            goto CloseFile;
        }

        bool hashSuccess = doHash(&appArgs, outF, hashCount);
        if(!hashSuccess)
        {
            ret = 2;
            goto CloseFile;
        }
    }
    while(appArgs.multi);

    ret = 0;

CloseFile:
    fclose(outF);
    if(inVal) free(inVal);

    return ret;
}
