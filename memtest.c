#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef SYSTEM_MALLOC
#define dev_free free
#define dev_malloc malloc
#define dev_realloc realloc
#else
#include "malloc.h"
#endif

int rand_between(int min, int max) { return rand() % (max - min + 1) + min; }

#define TEST_SIZE 50
#define LARGE_RANDOM_SIZE 1000000
#define REALLOC_SIZE 1000

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char *test_string = "Now is the time for all good people to come to the aid "
                      "of their country.";

    if (argc > 1) {
        test_string = argv[1];
    }

    char *ptrs[TEST_SIZE];

    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size = rand_between(1, strlen(test_string)+1);
        // coin flip for occasional large size
        if (rand_between(0, 100) < 20) {
            size = rand_between(1, LARGE_RANDOM_SIZE);
        }
        fprintf(stderr, "\n\n\n[%d] size: %d\n", ix, size);

        ptrs[ix] = dev_malloc(size);
        if (ptrs[ix] == NULL) {
            printf("[%d] malloc failed\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);

        // coin flip to occasionally realloc a random pointer
        if (rand_between(0, 100) < 20) {
            int new_size = rand_between(1, size + REALLOC_SIZE);
            fprintf(stderr, "\n[%d] reallocating %p to %d bytes\n", ix, ptrs[ix], new_size);
            ptrs[ix] = dev_realloc(ptrs[ix], new_size);
            fprintf(stderr, "[%d] reallocated %p\n", ix, ptrs[ix]);
            fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);
        }

        // coin flip to occasionally free a random pointer
        if (rand_between(0, 100) < 20) {
            int index_to_free = rand_between(0, ix);
            if (ptrs[index_to_free]) {
                fprintf(stderr, "\n[%d] randomly freeing %p ('%s')\n", index_to_free,
                        ptrs[index_to_free], ptrs[index_to_free]);
                dev_free(ptrs[index_to_free]);
                fprintf(stderr, "[%d] freed %p\n", index_to_free, ptrs[index_to_free]);
                ptrs[index_to_free] = NULL;
            }
        }
    }

    for (int ix = 0; ix < TEST_SIZE; ix++) {
        if (ptrs[ix]) {
            fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
            dev_free(ptrs[ix]);
            fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
        } else {
            fprintf(stderr, "[%d] already freed\n", ix);
        }
    }

    return 0;
}