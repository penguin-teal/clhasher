#include <stdio.h>
#include <string.h>
#include "algorithm.h"
#include "ints.h"
#include "appArgs.h"
#include "fnv.h"

size_t getHash(struct AppArgs *args, uint8_t *in, size_t size, uint8_t *outBuf, size_t bufSize)
{
    switch(args->bits)
    {
        case 32:
            if(bufSize >= sizeof(uint32_t))
            {
                uint32_t hash;
                switch(args->algorithm)
                {
                    case ALG_FNV0:
                        hash = fnv0_32(in, size);
                        break;
                    case ALG_FNV1:
                        hash = fnv1_32(in, size);
                        break;
                    case ALG_FNV1A:
                        hash = fnv1a_32(in, size);
                        break;
                    default:
                        return 0;
                }
                
                memcpy(outBuf, &hash, sizeof(uint32_t));
                return sizeof(uint32_t);
            }
            else
            {
                fprintf(stderr, "Buffer too small.\n");
                return 0;
            }
        case 64:
            if(bufSize >= sizeof(uint64_t))
            {
                uint64_t hash;
                switch(args->algorithm)
                {
                    case ALG_FNV0:
                        hash = fnv0_64(in, size);
                        break;
                    case ALG_FNV1:
                        hash = fnv1_64(in, size);
                        break;
                    case ALG_FNV1A:
                        hash = fnv1a_64(in, size);
                        break;
                    default:
                        return 0;
                }

                memcpy(outBuf, &hash, sizeof(uint64_t));
                return sizeof(uint64_t);
            }
            else
            {
                fprintf(stderr, "Buffer too small.\n");
                return 0;
            }
        case 128:
            if(bufSize >= sizeof(u128))
            {
                u128 hash;
                switch(args->algorithm)
                {
                    case ALG_FNV0:
                        hash = fnv0_128(in, size);
                        break;
                    case ALG_FNV1:
                        hash = fnv1_128(in, size);
                        break;
                    case ALG_FNV1A:
                        hash = fnv1a_128(in, size);
                        break;
                    default:
                        return 0;
                }
                
                memcpy(outBuf, &hash, sizeof(u128));
                return sizeof(u128);
            }
            else
            {
                fprintf(stderr, "Buffer too small.\n");
                return 0;
            }
        default:
            return 0;
    }
}
