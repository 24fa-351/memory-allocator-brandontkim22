all:  memtest_sys memtest_dev devmemtest_sys devmemtest_dev

memtest_sys: memtest.c
	gcc -o memtest_sys -DSYSTEM_MALLOC memtest.c

memtest_dev: memtest.c malloc.c malloc.h heap.c heap.h
	gcc -o memtest_dev memtest.c malloc.c heap.c

devmemtest_sys: devmemtest.c
	gcc -o devmemtest_sys -DSYSTEM_MALLOC devmemtest.c

devmemtest_dev: devmemtest.c devmalloc.c devmalloc.h devheap.c devheap.h
	gcc -o devmemtest_dev devmemtest.c devmalloc.c devheap.c

clean:
	rm -f memtest_sys memtest_dev
	rm -f devmemtest_sys devmemtest_dev