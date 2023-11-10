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
    int ret = 0;
    uint8_t *hashList = NULL;
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

    size_t hashSize = appArgs.bits / 8;

    uint64_t hashCount = 0;
    size_t hashListSize = 0;
    uint64_t hashListCount = 0;
    if(appArgs.multi)
    {
        hashListSize = 32 * hashSize;
        hashList = malloc(hashListSize);
        if(!hashList)
        {
            fprintf(stderr, "Couldn't malloc hash list.\n");
            ret = 1;
            goto CloseFile;
        }
    }
    else
    {
        hashListSize = 0;
    }

    bool gotInput;
    do
    {
        // Do some clean up if we are doing --multi
        if(appArgs.multi)
        {
            appArgs.value = NULL;
            if(inVal) free(inVal);
            inVal = NULL;
        }

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

        // Make sure hashList is big enough
        if(hashList && hashListSize - hashListCount < hashSize)
        {
            // Since the size was calculated as a multiple of hashSize
            // Double the size will always be enough
            hashListSize *= 2;
            uint8_t *newHashList = realloc(hashList, hashListSize);
            if(!newHashList)
            {
                fprintf(stderr, "Couldn't realloc hash list.\n");
                ret = 1;
                goto CloseFile;
            }
            else hashList = newHashList;
        }

        bool hashSuccess = doHash(&appArgs, outF, hashList + hashListCount, hashCount);
        if(!hashSuccess)
        {
            ret = 2;
            goto CloseFile;
        }

        hashListCount += hashSize;
    }
    while(appArgs.multi);

    if(appArgs.multi)
    {
        uint64_t collisionCount = 0;
        // 2 U64 indices for each hash
        size_t collisionIndicesSize = hashListCount / hashSize * sizeof(uint64_t) * 2;
        uint64_t *collisionIndices = malloc(collisionIndicesSize);

        for(uint64_t i = 0; i < hashListCount; i += hashSize)
        {
            uint8_t *hashA = hashList + i;
            for(uint64_t j = i + hashSize; j < hashListCount; j += hashSize)
            {
                uint8_t *hashB = hashList + j;
                if(!memcmp(hashA, hashB, hashSize))
                {
                    if(collisionCount * sizeof(uint64_t) * 2 >= collisionIndicesSize)
                    {
                        collisionIndicesSize *= 2;
                        uint64_t *newArr = realloc(collisionIndices, collisionIndicesSize);
                        if(!newArr)
                        {
                            fprintf(stderr, "Failed to realloc collision list.\n");
                            ret = 1;
                            goto ExitLoop;
                        }
                        collisionIndices = newArr;
                    }
                    collisionIndices[collisionCount * 2] = i / hashSize;
                    collisionIndices[collisionCount * 2 + 1] = j / hashSize;
                    collisionCount++;
                }
            }
        }
    ExitLoop:

        fprintf(outF, "\n%lu Collisions\n", collisionCount);
        if(collisionCount > 0) printf("\n");

        for(uint64_t i = 0; i < collisionCount; i++)
        {
            uint64_t hashInxA = collisionIndices[i * 2];
            uint64_t hashInxB = collisionIndices[i * 2 + 1];
            // TODO: Print out what those inputs actually are
            fprintf(outF, "Hashes #%lu and #%lu both get this hash:\n", hashInxA + 1, hashInxB + 1);
            printOut(hashList + hashInxA * hashSize, outF, &appArgs, hashInxA);
            fprintf(outF, "\n");
        }

        free(collisionIndices);
    }

CloseFile:
    fclose(outF);
    if(inVal) free(inVal);
    if(hashList) free(hashList);

    return ret;
}
