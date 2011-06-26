CC = gcc
LD = gcc
LDFLAGS = -Wall -O2 -std=c99
EXES = xxtea
TESTFILE= test.out

all: $(EXES)

% : %.c
	$(LD) $(LDFLAGS) -o $@ $<

clean: 
	rm -f $(EXES) $(TESTFILE)

tests: all
	./xxtea -o $(TESTFILE) test/in.1
	cmp $(TESTFILE) test/out.1
	$(RM) $(TESTFILE)
