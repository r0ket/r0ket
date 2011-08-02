CC = gcc
CFLAGS = -Wall -O2 -std=c99 -DSAFE
EXE = xxtea
FILES = main.c xxtea.c
OBJS = main.o xxtea.o

TESTFILE= test.out

all: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(EXE)

clean: 
	rm -f $(EXE) $(OBJS)
