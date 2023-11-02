#include <argp.h>
#include <stdio.h>
#include <stdbool.h>
#include "algorithm.h"
#include "appArgs.h"

enum OptKey
{
    OPT_VERBOSE         = 'v',
    OPT_OUT             = 'o',
    OPT_FNV1A           = -1,
    OPT_FNV1            = -2,
    OPT_FNV0            = -3,
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
    OPT_BASE10          = -9
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
    { "float"   ,       OPT_FLOAT           , 0     ,     0, "Prints the output as (a) 32-bit IEEE 754 float(s) binary32.", 1 },
    { "double"  ,       OPT_DOUBLE          , 0     ,     0, "Prints the output as (a) 64-bit IEEE 754 double float(s) binary64.", 1 },

    { 0         ,       0                   , 0     ,     0, "Print Number Format:", 2 },
    { "hex"     ,       OPT_HEX             , 0     ,     0, "Prints as (a) hexadecimal base-16 number(s).", 2 },
    { "octal"   ,       OPT_OCTAL           , 0     ,     0, "Prints as (a) octal base-8 number(s).", 2 },
    { "binary"  ,       OPT_BINARY          , 0     ,     0, "Prints as (a) binary base-2 number(s).", 2 },
    { "base-10" ,       OPT_BASE10          , 0     ,     0, "Default. Prints as (a) normal base-10 number(s).", 2 },
    
    { 0         ,       0                   , 0     ,     0, "Algorithms:", 4 },
    { "fnv1a"   ,       OPT_FNV1A           , 0     ,     0, "Default. Use FNV-1a hashing algorithm.", 4 },
    { "fnv1"    ,       OPT_FNV1            , 0     ,     0, "Use FNV-1 hashing algorithm.", 4 },
    { "fnv0"    ,       OPT_FNV0            , 0     ,     0, "Use FNV-0 hashing algorithm.", 4 },

    { 0         ,       0                   , 0     ,     0, "Hash Size:", 5 },
    { "b64"     ,       0                   , 0     ,     0, "Default. Produce a 64-bit hash.", 5 },
    { "b32"     ,       0                   , 0     ,     0, "Produce a 32-bit hash.", 5 },
    { "b128"    ,       0                   , 0     ,     0, "Produce a 128-bit hash.", 5 },


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

        case OPT_FNV1A:
            arguments->algorithm = ALG_FNV1A;
            break;
        case OPT_FNV1:
            arguments->algorithm = ALG_FNV1;
            break;
        case OPT_FNV0:
            arguments->algorithm = ALG_FNV0;
            break;
        
        case OPT_b64:
            arguments->bits = 64;
            break;
        case OPT_b32:
            arguments->bits = 32;
            break;
        case OPT_b128:
            arguments->bits = 16;
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
    appArgs->algorithm = ALG_FNV1A;
    appArgs->bits = 64;
    appArgs->splitBits = 64;
    appArgs->hiToLo = false;
    appArgs->radix = 10;

    error_t result = argp_parse(&argp, argc, argv, 0, 0, appArgs);

    if(!appArgs->splitBits || appArgs->splitBits > appArgs->bits)
    {
        appArgs->splitBits = appArgs->bits;
    }
    
    return !result;
}