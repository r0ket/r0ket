CC = gcc
CFLAGS = -Wall -O2 -std=c99 -DSAFE
EXE = xxtea
FILES = main.c xxtea.c
OBJS = main.o xxtea.o

TESTFILE= test.out

all: $(FILES) generate-keys.c 
	$(CC) $(CFLAGS) $(FILES) -o $(EXE)
	$(CC) $(CFLAGS) generate-keys.c -o generate-keys

clean: 
	rm -f $(EXE) $(OBJS)
