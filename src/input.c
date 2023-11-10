#include <stdio.h>
#include <string.h>
#include "appArgs.h"
#include "readF.h"

bool getInput(struct AppArgs *appArgs, char **inVal)
{
    if(!(appArgs->value))
    {
        if(appArgs->in)
        {
            if(appArgs->multi)
            {
                *inVal = mallocReadLineFile(stdin);
                if(!*inVal)
                {
                    appArgs->value = NULL;
                    return false;
                }
            }
            else if(!strncmp(appArgs->in, "-", 2) && !appArgs->len)
            {
                *inVal = mallocReadStdin(stdin);
                if(!*inVal)
                {
                    appArgs->value = NULL;
                    return false;
                }
            }
            else
            {
                FILE *fIn;
                if(!strncmp(appArgs->in, "-", 2)) fIn = stdin;
                else fIn = fopen(appArgs->in, "rb");
                
                if(!fIn)
                {
                    fprintf(stderr, "Couldn't open input file '%s'.\n", appArgs->in);
                    appArgs->value = NULL;
                    return false;
                }

                *inVal = mallocFileBuffer(fIn, appArgs->len);

                fclose(fIn);

                if(!*inVal)
                {
                    appArgs->value = NULL;
                    return false;
                }
            }

            appArgs->value = *inVal;
        }
        else
        {
            fprintf(stderr, "No value given to hash. Run with --usage to see usage.\n");
            return false;
        }
    }
    else if(appArgs->multi)
    {
        fprintf(stderr, "Must read data from a file or STDIN in --multi mode.\n");
        return false;
    }

    return true;
}

