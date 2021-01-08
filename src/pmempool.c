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
static  void touchpage(char *ptr, size_t size)
{
    for (long long i = 0; i < size ; i += 4096)
    {
        //long long y = ptr[i];
        ptr[i] = 0;
    }
    return;
}
/******************************************************************************/
/*
void create_pool()
{
    void *addr;
    char str[32];
    size_t mapped_len;

    size_t size = PMEMPOOL_SIZE;
    size_t alignment = CHUNK_SIZE;

    sprintf(str, "/mnt/pmem/dz_chunk_testfile");
    remove(str);
    if ((addr = mmap(NULL,size,PROT_READ | PROT_WRITE,MAP_ANON | MAP_SHARED,0,0)) == NULL)
    {
        perror("pmem_map_file");
        exit(1);
    }

    //alignment:chunksize
    if ((uintptr_t)addr == ALIGNMENT_CEILING((uintptr_t)addr, alignment))
    {
        pmempool.paddr = addr;
    }
    else
    {   
        munmap(addr,size);
        remove(str);
        if ((addr = mmap(NULL,size+CHUNK_SIZE,PROT_READ | PROT_WRITE,MAP_ANON | MAP_SHARED,0,0)) == NULL)
        {
            perror("pmem_map_file");
            exit(1);
        }
        pmempool.paddr = (void *)ALIGNMENT_CEILING((uintptr_t)addr, alignment);
        munmap(addr,((intptr_t)pmempool.paddr - (intptr_t)addr));
        munmap((void *)((intptr_t)pmempool.paddr + size), ((intptr_t)addr + CHUNK_SIZE - (intptr_t)pmempool.paddr));
    }

    pmempool.tailptr = pmempool.paddr;
}
*/

void create_pool()
{
    void *addr;
    char str[32];
    size_t mapped_len;
    int is_pmem;

    size_t size = PMEMPOOL_SIZE;
    size_t alignment = CHUNK_SIZE;

    sprintf(str, "/mnt/pmem/dz_chunk_testfile");
    remove(str);
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
    touchpage(pmempool.tailptr, PMEMPOOL_SIZE);
}

chunk_t *pool_alloc_chunk()
{
    chunk_t *ret = pool_global->tailptr;
    pool_global->tailptr = (void *)((intptr_t)pool_global->tailptr + CHUNK_SIZE);

    return ret;
}