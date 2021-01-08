#define CHUNK_C_
#include "chunk/internal/chunk_internal.h"
#include <x86intrin.h>
#include <libpmem.h>
__thread chunk_t *chunk_tls;

/******************************************************************************/
/* Data. */

/******************************************************************************/
/* Function prototypes for non-inline static functions. */

static void fill_chunktls()
{
    chunk_tls = pool_alloc_chunk();
    chunk_tls->splitp = (intptr_t)chunk_tls + CHUNK_SIZE;
    chunk_tls->tail_ptr = (region_t *)((intptr_t)chunk_tls + sizeof(chunk_t));
    return;
}

/******************************************************************************/
/* Inline tool function */

//regionsize:4B=32bit
//size:15  dirty:1  ptr:16
static inline uint32_t gen_attr_by(size_t size, uintptr_t ptr, unsigned short dirty)
{
#ifdef CHUNKDEBUG
    // printf("setattr:ptr:%lx ,size:%lx ,attr:%lx\n", ptr, size, (ptr << 16) | (size >> 2) | dirty);
#endif
    // return (ptr << 19) | (size >> 2) | dirty;
    return (size << 14) | (dirty << 16) | ptr ;  //todo: 17-3=14
}

static inline uintptr_t get_attr_ptr(uint32_t attr)
{
    return attr & ((1<<16)-1);
}

#define set_attr_state(attr, state) \
    do                              \
    {                               \
        attr |= state;              \
    } while (0)

/******************************************************************************/
int chunk_init()
{
    create_pool();
    fill_chunktls();
    return 1;
}

void *chunk_malloc(size_t size, lsptr *ptr)
{
    chunk_t *chunk = chunk_tls;
    size = ALIGNMENT_CEILING((size), ALIGNMENT);
    //todo 判断是否会填满

    if (chunk == NULL)
    {
        fill_chunktls();
        chunk = chunk_tls;
    }

    if ((intptr_t)chunk->tail_ptr + size + sizeof(region_t) > (intptr_t)chunk->splitp)
    {
        fill_chunktls();
        chunk = chunk_tls;
    }

    chunk->splitp -= size; //从下往上
    region_t *region = chunk->tail_ptr;
    region->attrs = gen_attr_by(size, (intptr_t)chunk->splitp - (intptr_t)region, REGION_CLEAN);
    //pmem_persist(chunk->tail_ptr,64);
    _mm_stream_si32(chunk->tail_ptr, gen_attr_by(size, (intptr_t)chunk->splitp - (intptr_t)chunk->tail_ptr, REGION_CLEAN));
    chunk->tail_ptr = (region_t *)((intptr_t)chunk->tail_ptr + sizeof(region_t)); //从上往下

    *ptr = (intptr_t)chunk->splitp - (intptr_t)ptr;
    return (void *)(chunk->splitp);
}

void chunk_free(lsptr *ptr)
{
    intptr_t splitaddr = (intptr_t)ptr + *ptr;

    intptr_t chunktailaddr = ALIGNMENT_CEILING(splitaddr, CHUNK_SIZE);
    chunk_t *chunk = (chunk_t *)(chunktailaddr - CHUNK_SIZE);
    int regionsize = sizeof(region_t);

    intptr_t left = (intptr_t)chunk + sizeof(chunk_t);
    intptr_t right = (intptr_t)chunk->tail_ptr - regionsize;
    intptr_t st = ALIGNMENT_CEILING((intptr_t)(1.0 * (chunktailaddr - splitaddr + 1e-6) / (chunktailaddr - chunk->splitp) * ((intptr_t)chunk->tail_ptr - left) + left), regionsize) - regionsize;
#ifdef CHUNKDEBUG
    //printf("st = %d\n", st);
#endif
    region_t *regionst = (region_t *)st;
    do
    {
#ifdef CHUNKDEBUG
        printf("st = %lld\n", st);
#endif  
        uint32_t at = regionst->attrs;
        intptr_t ptrr = (intptr_t)get_attr_ptr(at);
        intptr_t addr = (intptr_t)regionst + ptrr;

        if (addr == splitaddr)
        {
            uint32_t attr = regionst->attrs | REGION_DIRTY;
            _mm_stream_si32((int *)&regionst->attrs, attr);

            //set_attr_state(regionst->attrs, REGION_DIRTY);
            //_mm_clwb(&regionst->attrs);
#ifdef CHUNKDEBUG
            printf("found %llx %d\n", addr, *(int *)addr);
#endif
            break;
        }
        if (splitaddr > addr)
        {
            right = st - regionsize;
        }
        else
        {
            left = st + regionsize;
        }
        st = ALIGNMENT_CEILING((left + right) / 2, regionsize);
        regionst = (region_t *)st;
    } while (left <= right);
}
