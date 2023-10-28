#include <stdio.h>
#include "appArgs.h"

int main(int argc, char **argv)
{

    struct AppArgs appArgs;
    if(!doArgp(&appArgs, argc, argv)) return 2;

    printf("Hello, World!\n");
    return 0;
}