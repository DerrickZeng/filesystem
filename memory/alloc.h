#ifndef __MEMORY_ALLOC__
#define __MEMORY_ALLOC__

#include<stddef.h>


#define FT_NEW(type_, size) ((type_ *)ft_malloc0(sizeof(type_) * size))

void* ft_malloc(size_t size);
void* ft_malloc0(size_t size);
void* ft_realloc(void* p, size_t size);
void ft_free(void * p);


#endif /* __MEMORY_ALLOC__ */
