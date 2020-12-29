#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>
#include "chunk/chunk.h"

void *ptr[30000];

void do_test()
{

    struct timeval start;

    struct timeval end;

    unsigned long diff;

    gettimeofday(&start, NULL);

    for (int i = 0; i < 30000; i++)
        chunk_malloc(128, &ptr[i]);

    gettimeofday(&end, NULL);

    diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    printf("%lu\n", diff);

}

int main()
{
    chunk_init();

    do_test();

    return 0;
}