#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "appArgs.h"
#include "out.h"

bool reportCollisions(struct AppArgs *appArgs, uint64_t hashListCount, size_t hashSize, uint8_t *hashList, FILE *outF, string_list_T *allInputs)
{
    bool ret = true;

    if(appArgs->multi)
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
                            ret = false;
                            goto ExitCollisionLoop;
                        }
                        collisionIndices = newArr;
                    }
                    collisionIndices[collisionCount * 2] = i / hashSize;
                    collisionIndices[collisionCount * 2 + 1] = j / hashSize;
                    collisionCount++;
                }
            }
        }
    ExitCollisionLoop:

        fprintf(outF, "\n%lu Collisions\n", collisionCount);
        if(collisionCount > 0) printf("\n");

        for(uint64_t i = 0; i < collisionCount; i++)
        {
            uint64_t hashInxA = collisionIndices[i * 2];
            uint64_t hashInxB = collisionIndices[i * 2 + 1];

            char valA[19];
            char valB[19];
            size_t valASize = stringListSizeAt(allInputs, hashInxA);
            size_t valBSize = stringListSizeAt(allInputs, hashInxB);
            char *valARef = stringListAt(allInputs, hashInxA);
            char *valBRef = stringListAt(allInputs, hashInxB);

            // Put whole input or cut with '...' if needed
            if(valASize > sizeof valA - 1)
            {
                memcpy(valA, valARef, sizeof valA - 4);
                memset(valA + sizeof valA - 4, '.', 3);
            }
            else memcpy(valA, valARef, sizeof valA - 1);

            // Always add a NUL-terminator (this could result in double-NUL
            // but it doesn't matter)
            valA[sizeof valA - 1] = '\0';

            // same thing for B
            if(valBSize > sizeof valB - 1)
            {
                memcpy(valB, valBRef, sizeof valB - 4);
                memset(valB + sizeof valB - 4, '.', 3);
            }
            else memcpy(valB, valBRef, sizeof valB - 1);

            valB[sizeof valB - 1] = '\0';

            fprintf(outF, "Hashes #%lu (%.18s) and #%lu (%.18s) both get this hash:\n", hashInxA + 1, valA, hashInxB + 1, valB);
            printOut(hashList + hashInxA * hashSize, outF, appArgs, hashInxA);
            fprintf(outF, "\n");
        }

        free(collisionIndices);
    }

    return ret;

}
