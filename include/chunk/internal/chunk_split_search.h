/******************************************************************************/
#ifdef CHUNK_H_TYPES
typedef struct chunk_s chunk_t;
typedef struct region_s region_t;
#endif /* CHUNK_H_TYPES */
/******************************************************************************/
#ifdef CHUNK_H_STRUCTS

struct chunk_s
{
    region_t *tail_ptr;
    intptr_t splitp;
};

struct region_s
{
    uint64_t attrs;
};

#endif /* CHUNK_H_STRUCTS */
/******************************************************************************/
#ifdef CHUNK_H_EXTERNS

#endif /* CHUNK_H_EXTERNS */
/******************************************************************************/
#ifdef CHUNK_H_INLINES

#endif /* CHUNK_H_INLINES */
       /******************************************************************************/