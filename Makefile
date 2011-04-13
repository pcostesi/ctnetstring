IDIR = src/include
EDIR = lib/cds/src/include
CFLAGS = -I$(IDIR) -I$(EDIR)
CC = gcc

SRC= src/
TEST= test/
LIB= lib/

LIBS=-lcunit lib/cds/ds.a

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

all: test lib runtests

test: $(SRC)tns.o $(TEST)tns_test.o
	$(CC) -o $(TEST)test $(TEST)main.c $(SRC)tns.o $(TEST)tns_test.o $(LIBS) -I $(SRC)include 

runtests: test
	test/test

lib: $(SRC)tns.o
	ar rcs tns.a $(SRC)tns.o 

.PHONY: clean
 
clean:
	- rm $(SRC)*.o $(TEST)*.o tns.a $(TEST)test



