#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "appArgs.h"

void printOut(uint8_t *buf, FILE *f, struct AppArgs *appArgs)
{
    char fmt[6];
    fmt[0] = '%';
    char annoPrefix[3];

    uint64_t lines = appArgs->bits / appArgs->splitBits;

    char fmtCh;
    switch(appArgs->radix)
    {
        case 8:
            fmtCh = 'o';
            memcpy(annoPrefix, "0o", 3);
            break;
        case 10:
            fmtCh = 'u';
            annoPrefix[0] = '\0';
            break;
        case 16:
            fmtCh = 'X';
            memcpy(annoPrefix, "0x", 3);
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
                uint64_t inx;
                if(appArgs->hiToLo) inx = lines - i - 1;
                else inx = i;

                if(appArgs->annotate)
                {
                    fprintf(f, "u32 [%lu]:\t%s", inx * sizeof(uint32_t), annoPrefix);
                }
                fprintf(f, fmt, ((uint32_t*)buf)[inx]);
            }
            break;
        case 64:
            fmt[1] = 'l';
            fmt[2] = fmtCh;
            fmt[3] = '\n';
            fmt[4] = '\0';
            for(uint64_t i = 0; i < lines; i++)
            {
                uint64_t inx;
                if(appArgs->hiToLo) inx = lines - i - 1;
                else inx = i;

                if(appArgs->annotate)
                {
                    fprintf(f, "u64 [%lu]:\t%s", inx * sizeof(uint64_t), annoPrefix);
                }
                fprintf(f, fmt, ((uint64_t*)buf)[inx]);
            }
            break;
    }

}