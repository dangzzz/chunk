#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>
#include "chunk/chunk.h"

void *ptr[300000];



void do_test()
{



    struct timeval start;

    struct timeval end;

    unsigned long diff;

    gettimeofday(&start, NULL);
    for (int i = 0; i < 300000; i++)
        ptr[i] = chunk_malloc(69, &ptr[i]);
    gettimeofday(&end, NULL);

    diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    printf("malloc:%lu\n", diff);

    gettimeofday(&start, NULL);

    for (int i = 0; i < 300000; i++)
        chunk_free(ptr[i]);

    gettimeofday(&end, NULL);

    diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    printf("free:%lu\n", diff);
}

int main()
{

    chunk_init();
    do_test();

    return 0;
}