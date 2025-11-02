CC = gcc
CFLAGS = -lpthread

all: test-mergesort test-main

# gcc -lpthread   -c -o test-mergesort.o test-mergesort.c
# gcc -lpthread   -c -o mergesort.o mergesort.c
# gcc -lpthread -o test-mergesort test-mergesort.o mergesort.o

test-mergesort: test-mergesort.o mergesort.o
	$(CC) $(CFLAGS) -o $@ $?

test-main: main.o mergesort.o
	$(CC) $(CFLAGS) -o main $?

.PHONY: clean
clean:
	/bin/rm -f *.o *.d
	find . -maxdepth 1 -type f ! -name "*.*" ! -name "Makefile" -exec rm -f {} +

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
