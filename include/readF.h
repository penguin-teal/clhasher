#ifndef READSTDIN_H
#define READSTDIN_H

#include <stdio.h>

char *mallocReadStdin(FILE *f);

char *mallocReadLineFile(FILE *f);

char *mallocFileBuffer(FILE *f, size_t size);

#endif
