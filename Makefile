CC = gcc
CFLAGS = -Wall -Wpointer-arith -Wstrict-prototypes -std=gnu89 -fPIC -MMD -MP -lpthread

all: test-mergesort

#This builds an executable 
test-mergesort: test-mergesort.o mergesort.o
	$(CC) $(CFLAGS) -o $@ $?

.PHONY: clean
clean:
	/bin/rm -f *.o *.d test-mergesort

multitest:
	./test-mergesort 100000000 0 1234
	./test-mergesort 100000000 1 1234
	./test-mergesort 100000000 2 1234
	./test-mergesort 100000000 3 1234
	./test-mergesort 100000000 4 1234
	./test-mergesort 100000000 5 1234
	./test-mergesort 100000000 6 1234
	./test-mergesort 100000000 7 1234
	./test-mergesort 100000000 8 1234
