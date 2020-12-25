#ifndef CHUNK_INTERNAL_H
#define	CHUNK_INTERNAL_H



#  include <sys/param.h>
#  include <sys/mman.h>
#  include <sys/syscall.h>
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
#define	CHUNK_H_TYPES



#undef CHUNK_H_TYPES
/******************************************************************************/
#define CHUNK_H_STRUCTS




#undef CHUNK_H_STRUCTS 
/******************************************************************************/
#define CHUNK_H_EXTERNS




#undef  CHUNK_H_EXTERNS 
/******************************************************************************/
#define CHUNK_H_INLINES




#undef chunk_H_INLINES 
/******************************************************************************/
#endif /* CHUNK_INTERNAL_H */