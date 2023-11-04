#include <argp.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "algorithm.h"
#include "appArgs.h"

enum OptKey
{
    OPT_VERBOSE         = 'v',
    OPT_OUT             = 'o',
    OPT_FNV1A_32        = -1,
    OPT_FNV1A_64        = -10,
    OPT_FNV1A_128       = -11,
    OPT_FNV1_32         = -2,
    OPT_FNV1_64         = -12,
    OPT_FNV1_128        = -13,
    OPT_FNV0_32         = -3,
    OPT_FNV0_64         = -14,
    OPT_FNV0_128        = -15,
    OPT_b64             = -4,
    OPT_b32             = -5,
    OPT_b128            = -6,
    OPT_SPLIT_64        = 'q',
    OPT_SPLIT_32        = 'd',
    OPT_HI_TO_LO        = 'b',
    OPT_FLOAT           = 'l',
    OPT_DOUBLE          = 'f',
    OPT_HEX             = 'x',
    OPT_OCTAL           = -7,
    OPT_BINARY          = -8,
    OPT_BASE10          = -9,
    OPT_LEN             = 'z',
    OPT_HASH_NUL        = '0',
    OPT_ANNOTATE        = 'a'
};

const char *argp_program_version = "clhasher 1.0.0";
const char *argp_program_bug_address = "https://github.com/penguin-teal/clhasher";
static const char doc[] = "clhasher -- Quickly hash values.";
static const char argsDoc[] = "string";
static struct argp_option options[] =
{
    { 0         ,       0                   , 0     ,     0, "Output Options:", 1 },
    { "verbose" ,       OPT_VERBOSE         , 0     ,     0, "Describe what is happening.", 1 },
    { "out"     ,       OPT_OUT             , "FILE",     0, "Output to file. Defaults to '-' for STDOUT.", 1 },
    { "split-64",       OPT_SPLIT_64        , 0     ,     0, "Split the output to one 64-bit integer per line.", 1 },
    { "split-32",       OPT_SPLIT_32        , 0     ,     0, "Split the output to one 32-bit integer per line.", 1 },
    { "hi-to-lo",       OPT_HI_TO_LO        , 0     ,     0, "When splitting output, print the most significant part first.", 1 },
    //{ "float"   ,       OPT_FLOAT           , 0     ,     0, "Prints the output as (a) 32-bit IEEE 754 float(s) binary32.", 1 },
    //{ "double"  ,       OPT_DOUBLE          , 0     ,     0, "Prints the output as (a) 64-bit IEEE 754 double float(s) binary64.", 1 },
    { "length"  ,       OPT_LEN             , "SIZE",     0, "The size in bytes of the data to hash. Defaults to 0 which stops at NUL.", 1 },
    { "hash-nul",       OPT_HASH_NUL        , 0     ,     0, "When using auto length, includes the terminating NUL (\\0) in the hash.", 1 },
    { "annotate",       OPT_ANNOTATE        , 0     ,     0, "Annotate output with radix prefixes and index/size info.", 1 },

    { 0         ,       0                   , 0     ,     0, "Print Number Format:", 2 },
    { "hex"     ,       OPT_HEX             , 0     ,     0, "Prints as (a) hexadecimal base-16 number(s).", 2 },
    { "octal"   ,       OPT_OCTAL           , 0     ,     0, "Prints as (a) octal base-8 number(s).", 2 },
    //{ "binary"  ,       OPT_BINARY          , 0     ,     0, "Prints as (a) binary base-2 number(s).", 2 },
    { "base-10" ,       OPT_BASE10          , 0     ,     0, "Default. Prints as (a) normal base-10 number(s).", 2 },
    
    { 0          ,       0                   , 0     ,     0, "Algorithms:", 4 },
    { "fnv1a-32" ,       OPT_FNV1A_32        , 0     ,     0, "Default. Use FNV-1a 32-bit hashing algorithm.", 4 },
    { "fnv1a-64" ,       OPT_FNV1A_64        , 0     ,     0, "Default. Use FNV-1a 64-bit hashing algorithm.", 4 },
    { "fnv1a-128",       OPT_FNV1A_128       , 0     ,     0, "Default. Use FNV-1a 128-bit hashing algorithm.", 4 },
    { "fnv1-32"  ,       OPT_FNV1_32         , 0     ,     0, "Use FNV-1 32-bit hashing algorithm.", 4 },
    { "fnv1-64"  ,       OPT_FNV1_64         , 0     ,     0, "Use FNV-1 64-bit hashing algorithm.", 4 },
    { "fnv1-128" ,       OPT_FNV1_128        , 0     ,     0, "Use FNV-1 128-bit hashing algorithm.", 4 },
    { "fnv0-32"  ,       OPT_FNV0_32         , 0     ,     0, "Use FNV-0 32-bit hashing algorithm.", 4 },
    { "fnv0-64"  ,       OPT_FNV0_64         , 0     ,     0, "Use FNV-0 64-bit hashing algorithm.", 4 },
    { "fnv0-128" ,       OPT_FNV0_128        , 0     ,     0, "Use FNV-0 128-bit hashing algorithm.", 4 },

    { 0         ,       0                   , 0     ,     0, "Miscellaneous:", -1 },

    { 0 }
};

static error_t parseOpt(int key, char *arg, struct argp_state *state)
{
    struct AppArgs *arguments = state->input;

    switch(key)
    {
        case OPT_VERBOSE:
            arguments->verbose = true;
            break;
        case OPT_OUT:
            arguments->out = arg;
            break;
        case OPT_SPLIT_64:
            arguments->splitBits = 64;
            break;
        case OPT_SPLIT_32:
            arguments->splitBits = 32;
            break;
        case OPT_HI_TO_LO:
            arguments->hiToLo = true;
            break;
        case OPT_LEN:
            {
                int oldErrno = errno;
                errno = 0;

                char *end;
                uint64_t len = strtoul(arg, &end, 10);
                if(!len && errno)
                {
                    errno = oldErrno;
                    argp_usage(state);
                    break;
                }
                else
                {
                    errno = oldErrno;
                    arguments->len = len;
                    break;
                }
            }
        case OPT_HASH_NUL:
            arguments->hashNul = true;
            break;
        case OPT_ANNOTATE:
            arguments->annotate = true;
            break;

        case OPT_BINARY:
            arguments->radix = 2;
            break;
        case OPT_OCTAL:
            arguments->radix = 8;
            break;
        case OPT_HEX:
            arguments->radix = 16;
            break;
        case OPT_BASE10:
            arguments->radix = 10;
            break;

        case OPT_FNV1A_32:
            arguments->algorithm = ALG_FNV1A_32;
            break;
        case OPT_FNV1A_64:
            arguments->algorithm = ALG_FNV1A_64;
            break;
        case OPT_FNV1A_128:
            arguments->algorithm = ALG_FNV1A_128;
            break;
        case OPT_FNV1_32:
            arguments->algorithm = ALG_FNV1_32;
            break;
        case OPT_FNV1_64:
            arguments->algorithm = ALG_FNV1_64;
            break;
        case OPT_FNV1_128:
            arguments->algorithm = ALG_FNV1_128;
            break;
        case OPT_FNV0_32:
            arguments->algorithm = ALG_FNV0_32;
            break;
        case OPT_FNV0_64:
            arguments->algorithm = ALG_FNV0_64;
            break;
        case OPT_FNV0_128:
            arguments->algorithm = ALG_FNV0_128;
            break;
        
        case OPT_b64:
            arguments->bits = 64;
            break;
        case OPT_b32:
            arguments->bits = 32;
            break;
        case OPT_b128:
            arguments->bits = 128;
            break;
        
        case ARGP_KEY_ARG:
            if(state->arg_num > 1)
            {
                argp_usage(state);
            }
            arguments->value = arg;
            break;
        case ARGP_KEY_END:
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp =
{
    .options = options,
    .parser = parseOpt,
    .args_doc = argsDoc,
    .doc = doc
};

bool doArgp(struct AppArgs *appArgs, int argc, char **argv)
{
    appArgs->value = NULL;
    appArgs->verbose = false;
    appArgs->out = "-";
    appArgs->algorithm = 0;
    appArgs->bits = 64;
    appArgs->splitBits = 64;
    appArgs->hiToLo = false;
    appArgs->radix = 10;
    appArgs->len = 0;
    appArgs->hashNul = false;
    appArgs->annotate = false;

    error_t result = argp_parse(&argp, argc, argv, 0, 0, appArgs);

    appArgs->bits = appArgs->algorithm & ALG_SIZE_MASK;

    if(!appArgs->splitBits || appArgs->splitBits > appArgs->bits)
    {
        appArgs->splitBits = appArgs->bits;
    }
    
    return !result;
}