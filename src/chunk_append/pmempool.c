#define CHUNK_PMEMPOOL_C_
#include "chunk/internal/chunk_internal.h"
#include "libpmem.h"
/******************************************************************************/
/* Data. */
pool_t pmempool;
pool_t *pool_global = &pmempool;

/******************************************************************************/
/* Function prototypes for non-inline static functions. */

/******************************************************************************/
/* Inline tool function */

/******************************************************************************/

void create_pool()
{
    void *addr;
    char str[32];
    size_t mapped_len;
    int is_pmem;

    size_t size = PMEMPOOL_SIZE;
    size_t alignment = CHUNK_SIZE;

    sprintf(str, "/mnt/pmem/dz_chunk_testfile");

    if ((addr = pmem_map_file(str, size, PMEM_FILE_CREATE, 0666, &mapped_len, &is_pmem)) == NULL)
    {
        perror("pmem_map_file");
        exit(1);
    }
    if (!is_pmem)
    {
        perror("is_pmem");
        exit(1);
    }

    //alignment:chunksize
    if ((uintptr_t)addr == ALIGNMENT_CEILING((uintptr_t)addr, alignment))
    {
        pmempool.paddr = addr;
    }
    else
    {
        pmem_unmap(addr, size);
        remove(str);
        if ((addr = pmem_map_file(str, size + CHUNK_SIZE, PMEM_FILE_CREATE, 0666, &mapped_len, &is_pmem)) == NULL)
        {
            perror("pmem_map_file");
            exit(1);
        }
        pmempool.paddr = (void *)ALIGNMENT_CEILING((uintptr_t)addr, alignment);
        pmem_unmap(addr, ((intptr_t)pmempool.paddr - (intptr_t)addr));
        pmem_unmap((void *)((intptr_t)pmempool.paddr + size), ((intptr_t)addr + CHUNK_SIZE - (intptr_t)pmempool.paddr));
    }

    pmempool.tailptr = pmempool.paddr;
}

chunk_t *alloc_chunk()
{
    void *ret = pool_global->tailptr;
    pool_global->tailptr = (void *)((intptr_t)pool_global->tailptr + CHUNK_SIZE);
    return ret;
}