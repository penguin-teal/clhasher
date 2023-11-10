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
            if(!strncmp(appArgs->in, "-", 2) && !appArgs->len)
            {
                *inVal = mallocReadStdin(stdin);
                if(!inVal) return false;
            }
            else
            {
                FILE *fIn;
                if(!strncmp(appArgs->in, "-", 2)) fIn = stdin;
                else fIn = fopen(appArgs->in, "rb");
                
                if(!fIn)
                {
                    fprintf(stderr, "Couldn't open input file '%s'.\n", appArgs->in);
                    return false;
                }

                *inVal = mallocFileBuffer(fIn, appArgs->len);

                fclose(fIn);

                if(!inVal) return false;
            }

            appArgs->value = *inVal;
        }
        else
        {
            fprintf(stderr, "No value given to hash. Run with --usage to see usage.\n");
            return false;
        }
    }

    return true;
}

