#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "chunk/chunk.h"


int main()
{
    chunk_init();
    int *ptr = chunk_malloc(100, (void**)&ptr);
    *ptr = 100;
    printf("%d\n", *ptr);
    return 0;
}