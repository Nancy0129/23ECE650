CC=gcc
CFLAGS=-ggdb3 -fPIC
MALLOC_VERSION=BF
WDIR=..

all: mymalloc_test

mymalloc_test: mymalloc_test.c
	$(CC) $(CFLAGS) -I$(WDIR) -L$(WDIR) -D$(MALLOC_VERSION) -Wl,-rpath=$(WDIR) -o $@ mymalloc_test.c -l mymalloc -l rt

clean:
	rm -f *~ *.o mymalloc_test

clobber:
	rm -f *~ *.o
