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
    OPT_FNV0            = -3
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
    
    { 0         ,       0                   , 0     ,     0, "Algorithms:", 2 },
    { "fnv1a"   ,       OPT_FNV1A           , 0     ,     0, "Use FNV-1a hashing algorithm. Default.", 2 },
    { "fnv1"    ,       OPT_FNV1            , 0     ,     0, "Use FNV-1 hashing algorithm.", 2 },
    { "fnv0"    ,       OPT_FNV0            , 0     ,     0, "Use FNV-0 hashing algorithm.", 2 },

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

        case OPT_FNV1A:
            arguments->algorithm = ALG_FNV1A;
            break;
        case OPT_FNV1:
            arguments->algorithm = ALG_FNV1;
            break;
        case OPT_FNV0:
            arguments->algorithm = ALG_FNV0;
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
    appArgs->algorithm = OPT_FNV1A;

    error_t result = argp_parse(&argp, argc, argv, 0, 0, appArgs);
    return !result;
}