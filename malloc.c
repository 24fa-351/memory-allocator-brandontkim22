#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "malloc.h"
#include "heap.h"

#define MINIMUM_SBRK_SIZE 4096

freemem_t available_free_mem_chunks[NUMBER_OF_CHUNKS];
int heap_size = 0;

void* dev_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    size = size + sizeof(block_t);

    freemem_t min_block = extract_requested_block(available_free_mem_chunks, &heap_size, size);
    if (min_block.size != 0) {
        block_t* ptr = (block_t*) min_block.address;
        ptr->size = min_block.size;
        return ptr + 1;
    }

    // block not found, add a bigger chunk of memory to the available memory
    if (size < MINIMUM_SBRK_SIZE) {
        size = MINIMUM_SBRK_SIZE;
    }
    block_t* ptr = sbrk(size);
    ptr->size = size;

    return ptr + 1;
}

void dev_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    // gets the pointer of the header of the block
    block_t* block = (block_t*) ((char*) ptr - sizeof(block_t));

    freemem_t free_block = { 
        .size = block->size,
        .address = (char*) block
    };

    heap_insert(available_free_mem_chunks, &heap_size, free_block);
}

void* dev_realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return dev_malloc(size);
    }

    if (size == 0) {
        dev_free(ptr);
        return NULL;
    }

    block_t* block = (block_t*) ((char*) ptr - sizeof(block_t));
    if (block->size >= size) {
        freemem_t remaining_block = { 
            .size = block->size - size,
            .address = (char*) block + size
        };
        
        block->size = size;

        heap_insert(available_free_mem_chunks, &heap_size, remaining_block);

        return ptr;
    }

    void* new_ptr = dev_malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }

    block_t* new_block = (block_t*) ((char*) new_ptr - sizeof(block_t));
    memcpy(new_ptr, ptr, block->size);
    dev_free(ptr);
    return new_ptr;
}
