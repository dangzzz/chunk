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
    chunk_tls->avail_size = CHUNK_SIZE - ALIGNMENT_CEILING(sizeof(chunk_t), ALIGNMENT);
    chunk_tls->tail_ptr = (region_t *)((intptr_t)chunk_tls + ALIGNMENT_CEILING(sizeof(chunk_t), ALIGNMENT));
    return;
}

/******************************************************************************/
/* Inline tool function */

static inline uint64_t gen_attr_by(size_t size, uintptr_t ptr, unsigned short dirty)
{
#ifdef CHUNKDEBUG
    printf("setattr:ptr:%lx ,size:%lx ,attr:%lx\n", ptr, size, (ptr << 16) | (size >> 2) | dirty);
#endif
    return (ptr << 16) | (size >> 2) | dirty;
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

void *chunk_malloc(size_t size, void **ptr)
{
    chunk_t *chunk=chunk_tls;
    size = ALIGNMENT_CEILING((size + sizeof(region_t)), ALIGNMENT);

    if (size > chunk->avail_size)
    {
        fill_chunktls();
    }

    chunk = chunk_tls;

    chunk->avail_size -= size;
    region_t *region = chunk->tail_ptr;
    region->attrs = gen_attr_by(size, (uintptr_t)ptr, REGION_CLEAN);
    chunk->tail_ptr = (region_t *)((intptr_t)chunk->tail_ptr + size);
    return (void *)((intptr_t)region + sizeof(region_t));
}

void chunk_free(void *ptr)
{
    region_t *region = (region_t *)((intptr_t)ptr - sizeof(region_t));
    set_attr_state(region->attrs, REGION_DIRTY);
    return;
}
