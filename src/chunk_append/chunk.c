#define CHUNK_C_
#include "chunk/internal/chunk_internal.h"

__thread chunk_t *chunk_tls;

/******************************************************************************/
/* Data. */

/******************************************************************************/
/* Function prototypes for non-inline static functions. */

/******************************************************************************/
/* Inline tool function */

static inline uint64_t set_attr(size_t size, uintptr_t ptr, unsigned short dirty)
{
#ifdef CHUNKDEBUG
    printf("setattr:ptr:%lx ,size:%lx ,attr:%lx\n", ptr, size, (ptr << 16) | (size >> 2) | dirty);
#endif
    return (ptr << 16) | (size >> 2) | dirty;
}

/******************************************************************************/
void chunk_init()
{
    create_pool();
    chunk_tls = alloc_chunk();
}

void *chunk_malloc(size_t size, void **ptr)
{
    chunk_t *chunk = chunk_tls;
    size = ALIGNMENT_CEILING((size + sizeof(region_t)), ALIGNMENT);

#ifdef CHUNK_APPEND
    chunk->avail_size -= size;
    region_t *region = chunk->tail_ptr;
    region->attrs = set_attr(size, (uintptr_t)ptr, REGION_CLEAN);
    chunk->tail_ptr = (region_t *)((intptr_t)chunk->tail_ptr + size);
    return (void *)((intptr_t)region + sizeof(region_t));

#endif
}
