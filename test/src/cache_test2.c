#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>

#include "libpmem.h"
#define ALIGNMENT_CEILING(s, alignment) \
    (((s) + (alignment - 1)) & (-(alignment)))
size_t *ptr;

void do_test(int x)
{

    struct timeval start;

    struct timeval end;

    unsigned long diff;

    if (x == 1)
    {
        gettimeofday(&start, NULL);

        for (int i = 0; i < 1024 * 16 * 128; i++)
        {
            size_t y = ptr[i * 32];
            y = ptr[i * 32 + 4];
            y = ptr[i * 32 + 8];
            y = ptr[i * 32 + 12];
            y = ptr[i * 32 + 16];
            y = ptr[i * 32 + 20];
            y = ptr[i * 32 + 24];
            y = ptr[i * 32 + 28];
            /* write 2
            ptr[i * 32] = (size_t)i * x;
            pmem_persist(&ptr[i * 32], 8);
            ptr[i * 32 + 4] = (size_t)i * x;
            pmem_persist(&ptr[i * 32 + 4], 8);
            ptr[i * 32 + 8] = (size_t)i * x;
            pmem_persist(&ptr[i * 32 + 8], 8);
            ptr[i * 32 + 12] = (size_t)i * x;
            pmem_persist(&ptr[i * 32 + 12], 8);
            ptr[i * 32 + 16] = (size_t)i * x;
            pmem_persist(&ptr[i * 32 + 16], 8);
            ptr[i * 32 + 20] = (size_t)i * x;
            pmem_persist(&ptr[i * 32 + 20], 8);
            ptr[i * 32 + 24] = (size_t)i * x;
            pmem_persist(&ptr[i * 32 + 24], 8);
            ptr[i * 32 + 28] = (size_t)i * x;
            pmem_persist(&ptr[i * 32 + 28], 8);
            */
            /*
            ptr[i * 32] = (size_t)i * x;
            ptr[i * 32 + 4] = (size_t)i * x;
            ptr[i * 32 + 8] = (size_t)i * x;
            ptr[i * 32 + 12] = (size_t)i * x;
            ptr[i * 32 + 16] = (size_t)i * x;
            ptr[i * 32 + 20] = (size_t)i * x;
            ptr[i * 32 + 24] = (size_t)i * x;
            ptr[i * 32 + 28] = (size_t)i * x;
            */
            /*write 3
            pmem_persist(&ptr[i * 32], 256);
            */
        }

        gettimeofday(&end, NULL);

        diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

        printf("no cpu cache:%lu\n", diff);
    }
    else if (x == 2)
    {
        gettimeofday(&start, NULL);

        for (int i = 0; i < 1024 * 16 * 128; i++)
        {
            size_t y = ptr[i * 32];
            y = ptr[i * 32 + 1];
            y = ptr[i * 32 + 2];
            y = ptr[i * 32 + 3];
            y = ptr[i * 32 + 4];
            y = ptr[i * 32 + 5];
            y = ptr[i * 32 + 6];
            y = ptr[i * 32 + 7];
            /*
            ptr[i * 32] = (size_t)i * x;
            ptr[i * 32 + 1] = (size_t)i * x;
            ptr[i * 32 + 2] = (size_t)i * x;
            ptr[i * 32 + 3] = (size_t)i * x;
            ptr[i * 32 + 4] = (size_t)i * x;
            ptr[i * 32 + 5] = (size_t)i * x;
            ptr[i * 32 + 6] = (size_t)i * x;
            ptr[i * 32 + 7] = (size_t)i * x;
            pmem_persist(&ptr[i * 32], 64);
            */
        }

        gettimeofday(&end, NULL);

        diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

        printf("cpu cache:%lu\n", diff);
    }
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
    //ptr = mmap(NULL, 8 * 1024 * 1024 * 128, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, 0, 0);

    if ((intptr_t)ptr % 4 * 1024 != 0)
    {
        ptr = (size_t *)ALIGNMENT_CEILING(((intptr_t)ptr), 4 * 1024);
    }

    for (int i = 0; i < 1024 * 1024 * 128; i++)
    {
        ptr[i] = 0x3ff131ff;
    }

    do_test(x);

    return 0;
}