#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>

#include "libpmem.h"
#define ALIGNMENT_CEILING(s, alignment) \
    (((s) + (alignment - 1)) & (-(alignment)))
size_t *ptr;

void do_test(int x)
{

    struct timeval start;

    struct timeval end;

    unsigned long diff;

    gettimeofday(&start, NULL);

    for (int i = 0; i < 1024 * 128; i++)
    {
        //size_t y = ptr[ i * x];
        ptr[i * x] = (size_t)i * x;
        //pmem_persist(&ptr[i * x],8);
    }

    gettimeofday(&end, NULL);

    diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    printf("%d bytes:%lu\n", 8 * x, diff);
}

int main(int argc, char *argv[])
{
    char str[32];
    int x = atoi(argv[1]);
    sprintf(str, "/mnt/pmem/dz_chunk_testfile%d", x);
    size_t mapped_len;
    int is_pmem;
    if ((ptr = pmem_map_file(str, 8 * 1024 * 1024 * 128, PMEM_FILE_CREATE, 0666, &mapped_len, &is_pmem)) == NULL)
    {
        perror("pmem_map_file");
        exit(1);
    }
    if (!is_pmem)
    {
        perror("is_pmem");
        exit(1);
    }

    if ((intptr_t)ptr % 64 != 0)
    {
        ptr = (size_t *)ALIGNMENT_CEILING(((intptr_t)ptr), 64);
    }

    for (int i = 0; i < 1024 * 1024 * 128; i++)
    {
        ptr[i] = 0x3ff131ff + x;
    }

    do_test(x);

    return 0;
}