#define CHUNK_C_
#include "chunk/internal/chunk_internal.h"

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

static inline uint64_t gen_attr_by(size_t size, uintptr_t ptr)
{
#ifdef CHUNKDEBUG
    printf("setattr:ptr:%lx ,size:%lx ,attr:%lx\n", ptr, size, (ptr << 16) | (size >> 2));
#endif
    return (ptr << 16) | (size >> 2);
}

#define set_attr_state(attr, state) \
    do                              \
    {                               \
        attr |= state;              \
    } while (0)

/******************************************************************************/
void chunk_init()
{
    create_pool();
    fill_chunktls();
}

void *chunk_malloc(size_t size, void **ptr)
{
    chunk_t *chunk=chunk_tls;
    size = ALIGNMENT_CEILING((size + sizeof(follower_t)), ALIGNMENT);
    
    if((intptr_t)chunk->tail_ptr+size+sizeof(region_t)>(intptr_t)chunk->splitp){
        fill_chunktls();
    }

    chunk = chunk_tls;

    chunk->splitp -= size;
    region_t *region = chunk->tail_ptr;
    region->attrs = gen_attr_by(size, (uintptr_t)ptr);
    chunk->tail_ptr = (region_t *)((intptr_t)chunk->tail_ptr + sizeof(region_t));
    return (void *)((intptr_t)chunk->splitp + sizeof(follower_t));
}

void chunk_free(void *ptr)
{
    follower_t *fo = (follower_t *)((intptr_t)ptr - sizeof(follower_t));
    set_attr_state(fo->attrs, REGION_DIRTY);
    return;
}
