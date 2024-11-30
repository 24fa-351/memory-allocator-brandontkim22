#include <stdlib.h>

#include "malloc.h"

#ifndef HEAP_H
#define HEAP_H

#define NUMBER_OF_CHUNKS 1000

typedef struct block {
    size_t size; // total size of memory including header size
} block_t;

typedef struct freemem {
    size_t size; // total size of memory including header size
    char* address; // address of the block
} freemem_t;

void heap_insert(freemem_t heap[], int* heap_size, freemem_t block);

freemem_t extract_requested_block(freemem_t heap[], int* heap_size, size_t size);

#endif
