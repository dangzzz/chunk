#ifndef CHUNK_H_
#define	CHUNK_H_
#ifdef __cplusplus
extern "C" {
#endif

void * chunk_malloc(size_t size, void** ptr);
void chunk_free(void *ptr);




#ifdef __cplusplus
};
#endif
#endif /* CHUNK_H_ */