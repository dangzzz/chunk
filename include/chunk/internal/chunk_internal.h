#ifndef CHUNK_INTERNAL_H
#define CHUNK_INTERNAL_H

#include <sys/param.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "chunk/chunk.h"

/*
 * There are circular dependencies that cannot be broken without
 * substantial performance degradation.  In order to reduce the effect on
 * visual code flow, read the header files in multiple passes, with one of the
 * following cpp variables defined during each pass:
 *
 *   CHUNK_H_TYPES   : Preprocessor-defined constants and psuedo-opaque data
 *                        types.
 *   CHUNK_H_STRUCTS : Data structures.
 *   CHUNK_H_EXTERNS : Extern data declarations and function prototypes.
 *   CHUNK_H_INLINES : Inline functions.
 */
/******************************************************************************/
#define CHUNK_H_TYPES

#define CHUNK_LG_SIZE 16
#define ALIGNMENT ((size_t)8)
#define PMEMPOOL_SIZE ((size_t)160 * 1024 * 1024)

#define CHUNK_SIZE (((size_t)1) << CHUNK_LG_SIZE)

#define ALIGNMENT_CEILING(s, alignment) \
    (((s) + (alignment - 1)) & (-(alignment)))
#include "chunk/internal/lsptr.h"
#include "chunk/internal/pmempool.h"
#ifdef CHUNK_APPEND
#define REGION_CLEAN 0
#define REGION_DIRTY 1

#include "chunk/internal/chunk_append.h"
#elif defined CHUNK_BITMAP
#include "chunk/internal/chunk_bitmap.h"
#elif defined CHUNK_SPLIT_FOLLOW
#define REGION_CLEAN 0
#define REGION_DIRTY 1
#include "chunk/internal/chunk_split_follow.h"
#elif defined CHUNK_SPLIT_SEARCH
#define REGION_CLEAN 0
#define REGION_DIRTY 1
#include "chunk/internal/chunk_split_search.h"

#endif

#undef CHUNK_H_TYPES
/******************************************************************************/
#define CHUNK_H_STRUCTS
#include "chunk/internal/lsptr.h"
#include "chunk/internal/pmempool.h"
#ifdef CHUNK_APPEND
#include "chunk/internal/chunk_append.h"
#elif defined CHUNK_BITMAP
#include "chunk/internal/chunk_bitmap.h"

#elif defined CHUNK_SPLIT_FOLLOW
#define REGION_CLEAN 0
#define REGION_DIRTY 1
#include "chunk/internal/chunk_split_follow.h"
#elif defined CHUNK_SPLIT_SEARCH
#define REGION_CLEAN 0
#define REGION_DIRTY 1
#include "chunk/internal/chunk_split_search.h"
#endif

#undef CHUNK_H_STRUCTS
/******************************************************************************/
#define CHUNK_H_EXTERNS
extern pool_t *pool_global;
#include "chunk/internal/lsptr.h"
#include "chunk/internal/pmempool.h"
#ifdef CHUNK_APPEND
#include "chunk/internal/chunk_append.h"
#elif defined CHUNK_BITMAP
#include "chunk/internal/chunk_bitmap.h"

#elif defined CHUNK_SPLIT_FOLLOW
#define REGION_CLEAN 0
#define REGION_DIRTY 1
#include "chunk/internal/chunk_split_follow.h"
#elif defined CHUNK_SPLIT_SEARCH
#define REGION_CLEAN 0
#define REGION_DIRTY 1
#include "chunk/internal/chunk_split_search.h"

#endif

#undef CHUNK_H_EXTERNS
/******************************************************************************/
#define CHUNK_H_INLINES
#include "chunk/internal/lsptr.h"
#include "chunk/internal/pmempool.h"
#ifdef CHUNK_APPEND
#include "chunk/internal/chunk_append.h"
#elif defined CHUNK_BITMAP
#include "chunk/internal/chunk_bitmap.h"

#elif defined CHUNK_SPLIT_FOLLOW
#define REGION_CLEAN 0
#define REGION_DIRTY 1
#include "chunk/internal/chunk_split_follow.h"
#elif defined CHUNK_SPLIT_SEARCH
#define REGION_CLEAN 0
#define REGION_DIRTY 1
#include "chunk/internal/chunk_split_search.h"

#endif

#undef chunk_H_INLINES
/******************************************************************************/
#endif /* CHUNK_INTERNAL_H */