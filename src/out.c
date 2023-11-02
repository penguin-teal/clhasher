#include <stdio.h>
#include <stdint.h>
#include "appArgs.h"

void printOut(uint8_t *buf, FILE *f, struct AppArgs *appArgs)
{
    char fmt[6];
    fmt[0] = '%';

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
            fmt[1] = fmtCh;
            fmt[2] = '\n';
            fmt[3] = '\0';
            for(uint64_t i = 0; i < lines; i++)
            {
                fprintf(f, fmt, ((uint32_t*)buf)[i]);
            }
            break;
        case 64:
            fmt[1] = 'l';
            fmt[2] = fmtCh;
            fmt[3] = '\n';
            fmt[4] = '\0';
            for(uint64_t i = 0; i < lines; i++)
            {
                fprintf(f, fmt, ((uint64_t*)buf)[i]);
            }
            break;
    }

}