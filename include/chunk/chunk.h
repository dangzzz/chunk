#ifndef CHUNK_H_
#define	CHUNK_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef intptr_t lsptr;

#define toptr(type,lsptr)    ((type)((intptr_t)&lsptr+(intptr_t)lsptr))   


void * chunk_malloc(size_t size, lsptr * ptr);
void chunk_free(lsptr * ptr);
// void * chunk_malloc(size_t size, void ** ptr);
// void chunk_free(void * ptr);
int chunk_init();



#ifdef __cplusplus
};
#endif
#endif /* CHUNK_H_ */