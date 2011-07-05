##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =

OBJS += nrf24l01p.o

LIBNAME=funk

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH?= ..
INCLUDE_PATHS = -I$(ROOT_PATH) -I../core -I.

include $(ROOT_PATH)/Makefile.inc

LIBFILE=lib$(LIBNAME).a
##########################################################################
# Compiler settings, parameters and flags
##########################################################################

all: $(LIBFILE)

$(LIBFILE): $(OBJS)
	$(AR) rcs $@ $(OBJS)
	
%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(LIBFILE)

nrf24l01p.o: nrf24l01p.h

