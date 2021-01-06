/******************************************************************************/
#ifdef CHUNK_H_TYPES
typedef struct pool_s pool_t;
#endif /* CHUNK_H_TYPES */
/******************************************************************************/
#ifdef CHUNK_H_STRUCTS

struct pool_s
{
    void *paddr;
    void *tailptr;
};

#endif /* CHUNK_H_STRUCTS */
/******************************************************************************/
#ifdef CHUNK_H_EXTERNS

void create_pool();
chunk_t *pool_alloc_chunk();

#endif /* CHUNK_H_EXTERNS */
/******************************************************************************/
#ifdef CHUNK_H_INLINES

#endif /* CHUNK_H_INLINES */
       /******************************************************************************/