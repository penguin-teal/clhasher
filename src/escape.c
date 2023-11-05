#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

static size_t escNums(char *out, const char **in, size_t z, int radix)
{
    if(z > 31)
    {
        fprintf(stderr, "Escape sequence size too large '%lu' B.", z);
        return 0;
    }

    char buf[32];
    const char *last = *in;
    
    memcpy(buf, last, z);
    buf[z] = '\0';
    *in += z;

    int lastErrno = errno;
    errno = 0;

    char *end;
    uint64_t u = strtoul(buf, &end, radix);

    int gotErrno = errno;
    errno = lastErrno;

    if (gotErrno || (size_t)(end - buf) != strlen(buf))
    {
        fprintf(stderr, "Malformed octal escape sequence '%s'.", buf);
        if(gotErrno) fprintf(stderr, " Errno 0x%x: %s.\n", gotErrno, strerror(gotErrno));
        else fprintf(stderr, "\n");
        return 0;
    }

    memcpy(out, &u, sizeof(char));
    return 1;
}

char *mallocEscStr(const char *in)
{
    const char *atIn = in;
    const char *lastIn = in;
    size_t inLen = strlen(in);
    char *s = malloc(inLen);
    char *atOut = s;

    size_t advance = 0;
    while((atIn = strchr(lastIn, '\\')))
    {
        memcpy(atOut, lastIn, atIn - lastIn);
        atOut += (atIn - lastIn);
        lastIn = atIn + 1;
        switch(*lastIn)
        {
            case '\\':
                atOut[0] = '\\';
                atOut++;
                lastIn++;
                break;
            case 'n':
                atOut[0] = '\n';
                atOut++;
                lastIn++;
                break;
            case 'r':
                atOut[0] = '\r';
                atOut++;
                lastIn++;
                break;
            case 't':
                atOut[0] = '\t';
                atOut++;
                lastIn++;
                break;
            case '0':
                if(!isdigit((++lastIn)[1]))
                {
                    atOut[0] = '\0';
                    atOut++;
                    break;
                }
                // FALL THRU
            case 'o':
                lastIn++;
                // FALL THRU
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                advance = escNums(atOut, &lastIn, 3, 8);
                if(!advance) goto Fail;
                atOut += advance;
                break;
            case 'x':
                lastIn++;
                advance = escNums(atOut, &lastIn, 2, 16);
                if(!advance) goto Fail;
                atOut += advance;
                break;
            default:
                fprintf(stderr, "Unrecorgnized escape sequence '\\%c'.\n", *lastIn);
                goto Fail;
        }
    }

    if((size_t)(lastIn - in) < inLen)
    {
        size_t z = inLen - (lastIn - in);
        memcpy(atOut, lastIn, z);
        atOut += z;
    }

    atOut[0] = '\0';

    return s;

Fail:
    free(s);
    return NULL;
}