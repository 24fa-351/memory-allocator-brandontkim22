#include <stdlib.h>

#ifndef MALLOC_H
#define MALLOC_H

void* dev_malloc(size_t size);

void dev_free(void* ptr);

void* dev_realloc(void* ptr, size_t size);

#endif