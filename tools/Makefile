CC = gcc
LD = gcc
LDFLAGS = -Wall -O4 -std=c99
EXES = lpcrc

all: $(EXES)

% : %.c
	$(LD) $(LDFLAGS) -o $@ $<

clean: 
	rm -f $(EXES)
