#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "heap.h"

#define HEAP_KEY_FORMAT "%zu"

unsigned int heap_parent(unsigned int index) { return (index - 1) / 2; }

unsigned int heap_left_child(unsigned int index) { return (index * 2) + 1; }

unsigned int heap_right_child(unsigned int index) { return (index * 2) + 2; }

unsigned int heap_level(unsigned int index) {
    return (unsigned int)log2(index + 1);
}

void heap_print(freemem_t heap[], int* heap_size) {
    for (int ix = 0; ix < *heap_size; ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT ", address: %p\n", heap_level(ix), ix,
               heap[ix].size, heap[ix].address);
        // printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
        //        heap[ix].size);
    }
    printf("\n");
}

void heap_swap(freemem_t heap[], int* index1, int* index2) {
    freemem_t temp = heap[*index1];
    heap[*index1] = heap[*index2];
    heap[*index2] = temp;
}

void heap_bubble_up(freemem_t heap[], int* index) {
    int current = *index;
    while (heap_level(current) > 0) {
        int parent = heap_parent(current);

        if (heap[current].size > heap[parent].size) {
            break;
        }

        heap_swap(heap, &current, &parent);
        current = parent;
    }
}

void heap_bubble_down(freemem_t heap[], int* heap_size, int current) {
    while (heap_left_child(current) < *heap_size) {
        int left_child = heap_left_child(current);
        int right_child = heap_right_child(current);
        int smallest = left_child;

        if (right_child < *heap_size && heap[right_child].size < heap[smallest].size) {
            smallest = right_child;
        }

        if (current < smallest) {
            break;
        }

        heap_swap(heap, &current, &smallest);
        current = smallest;
    }
}

void heap_insert(freemem_t heap[], int* heap_size, freemem_t block) {
    if (*heap_size == NUMBER_OF_CHUNKS) {
        fprintf(stderr, "Heap is full\n");
        return;
    }
    
    heap[*heap_size] = block;
    heap_bubble_up(heap, heap_size);
    *heap_size = *heap_size + 1;

    heap_print(heap, heap_size);
}

freemem_t extract_requested_block(freemem_t heap[], int* heap_size, size_t size) {
    if (*heap_size == 0) {
        fprintf(stderr, "Heap is empty\n");
        return (freemem_t) {0, NULL};
    }
    
    for (int ix = 0; ix < *heap_size; ix++) {
        if (heap[ix].size >= size) {
            if (heap[ix].size == size) {
                freemem_t block = heap[ix];
                
                // remove the block from the free heap
                *heap_size = *heap_size - 1;
                heap[ix] = heap[*heap_size];
                heap_bubble_down(heap, heap_size, ix);
                
                return block;
            }
            // block is larger than needed, split the block
            freemem_t block = heap[ix];
            freemem_t remaining_block = { 
                .size = block.size - size,
                .address = block.address + size
                };
            // update the block we are returning to size requested
            block.size = size;
            
            // remove the block from the free heap
            *heap_size = *heap_size - 1;
            heap[ix] = heap[*heap_size];
            heap_bubble_down(heap, heap_size, ix);
            
            heap_insert(heap, heap_size, remaining_block);
            
            heap_print(heap, heap_size);

            return block;
        }
    }
    
    return (freemem_t) {0, NULL};
}


