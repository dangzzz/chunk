#include "chunk/chunk.h"
#include <stdio.h>

int main()
{
    chunk_init();
    int *ptr = chunk_malloc(100, &ptr);
    *ptr = 100;
    printf("%d\n", *ptr);
    return;
}