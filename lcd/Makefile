##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =
OBJS += smallfonts.o

OBJS += ubuntu18.o
OBJS += orbitron14.o

OBJS += display.o
OBJS += render.o
OBJS += decoder.o

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH?= ..
INCLUDE_PATHS = -I$(ROOT_PATH) -I../core -I.

include $(ROOT_PATH)/Makefile.inc

##########################################################################
# Compiler settings, parameters and flags
##########################################################################

all: libfont.a

libfont.a: $(OBJS)
	$(AR) rcs libfont.a $(OBJS)
	
%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) libfont.a

render.o: render.c render.h display.h fonts.h
smallfonts.o: smallfonts.c smallfonts.h fonts.h

