#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "stdint.h"
#include "chunk/chunk.h"

int main()
{
    chunk_init();
    // lsptr ptr;
    // int * addr = chunk_malloc(100, &ptr);
    // *addr = 100;
    // printf("%d\n", *addr);
    // printf("%lld\n", addr);
    // printf("%lld\n", ptr);
    // printf("%lld\n", &ptr);
    // printf("---free---\n");
    // chunk_free(&ptr);

    lsptr ptr[10];
    int * addr[10];
    for (int i = 0; i < 10; i++)
    {
        printf("%d:  ", i);
        addr[i] = chunk_malloc(100, &ptr[i]);
        *addr[i] = 10*i;
        printf("%d ", *addr[i]);
        printf("%lld ", addr[i]);
        printf("%lld ", ptr[i]);
        printf("%lld\n", &ptr[i]);
    }

    int x[10] = {2, 9, 8, 0, 6, 7, 5, 3, 4, 1};
    printf("----free----\n");
    for (int i = 0; i < 10; i++)
    {
        chunk_free(&ptr[x[i]]);
    }

    return 0;
}



