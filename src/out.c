#include <stdio.h>
#include <stdint.h>
#include "appArgs.h"

void printOut(uint8_t *buf, FILE *f, struct AppArgs *appArgs)
{
    static char *fmt32 = "%u\n";
    static char *fmt64 = "%lu\n";

    uint64_t lines = appArgs->bits / appArgs->splitBits;

    char fmtCh;
    switch(appArgs->radix)
    {
        case 8:
            fmtCh = 'o';
            break;
        case 10:
            fmtCh = 'u';
            break;
        case 16:
            fmtCh = 'X';
            break;
        default:
            printf("Radix %u unsupported.\n", appArgs->radix);
            return;
    }

    switch(appArgs->splitBits)
    {
        case 32:
            fmt32[1] = fmtCh;
            for(uint64_t i = 0; i < lines; i++)
            {
                fprintf(f, fmt32, ((uint32_t*)buf)[i]);
            }
            break;
        case 64:
            fmt64[2] = fmtCh;
            for(uint64_t i = 0; i < lines; i++)
            {
                fprintf(f, fmt64, ((uint64_t*)buf)[i]);
            }
            break;
    }

}