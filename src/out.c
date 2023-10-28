#include <stdio.h>
#include <stdint.h>
#include "appArgs.h"

void printOut(uint8_t *buf, FILE *f, struct AppArgs *appArgs)
{
    uint64_t lines = appArgs->bits / appArgs->splitBits;

    switch(appArgs->splitBits)
    {
        case 32:
            for(uint64_t i = 0; i < lines; i++)
            {
                fprintf(f, "%u\n", ((uint32_t*)buf)[i]);
            }
            break;
        case 64:
            for(uint64_t i = 0; i < lines; i++)
            {
                fprintf(f, "%lu\n", ((uint64_t*)buf)[i]);
            }
            break;
    }

}