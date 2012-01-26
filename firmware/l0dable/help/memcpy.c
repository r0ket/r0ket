#include <string.h>
#include "../usetable.h"

#undef memcpy

void * memcpy(void *dst, const void *src, size_t len){
	return
		(*(void * (*)(void *, const void *, size_t ))(*(TheTable+_memcpy_nr)))
		(dst, src, len);
};
