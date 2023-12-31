#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#define POSIX
#include <unistd.h>
#include <sys/stat.h>
#endif

char *mallocReadStdin(FILE *f)
{
    size_t z = 1024;
    char *s = malloc(z);

    char *end = fgets(s, z, f);

    if(!end)
    {
        fprintf(stderr, "Value is required. Run with --usage to see usage.\n");
        free(s);
        return NULL;
    }

    return s;
}

char *mallocReadLineFile(FILE *f)
{
    char *line = NULL;
    size_t z = 0;
    ssize_t count = getline(&line, &z, f);
    if(count == -1)
    {
        free(line);
        return NULL;
    }
    else
    {
        // Replace trailing '\n' with a NUL so we don't hash it
        if(count > 0 && line[count - 1] == '\n') line[count - 1] = '\0';
        return line;
    }
}

static size_t getFileSize(FILE *f)
{
#ifdef POSIX

    struct stat statStruct;
    int descriptor = fileno(f);
    fstat(descriptor, &statStruct);
    return statStruct.st_size;

#elif

    fseek(f, 0, SEEK_END);
    size_t z = ftell(f);
    fseek(f, 0, SEEK_SET);
    return z;

#endif
}

char *mallocFileBuffer(FILE *f, size_t size)
{
    if(!size)
    {
        size = getFileSize(f);
    }

    char *s = malloc(size + 1);
    if(!s)
    {
        fprintf(stderr, "Couldn't allocate file buffer.\n");
        free(s);
        return NULL;
    }

    size_t c = fread(s, size, sizeof(char), f);

    if(!c)
    {
        fprintf(stderr, "Couldn't read from file stream.\n");
        free(s);
        return NULL;
    }

    s[c] = '\0';

    return s;
}
