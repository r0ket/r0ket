##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =

OBJS += display.o
OBJS += render.o
OBJS += decoder.o
OBJS += backlight.o
OBJS += print.o

FONTS = $(basename $(wildcard fonts/*.c))

LIBNAME=lcd

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH?= ..
INCLUDE_PATHS = -I$(ROOT_PATH) -I../core -I.

include $(ROOT_PATH)/Makefile.inc

FOBJS= $(foreach ft,$(FONTS),$(ft).o)
OBJS+= $(FOBJS)
LIBFILE=lib$(LIBNAME).a
##########################################################################
# Compiler settings, parameters and flags
##########################################################################

all: $(LIBFILE) allfonts.h

$(LIBFILE): $(OBJS)
	$(AR) rcs $@ $(OBJS)
	
%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

$(FOBJS): $(foreach ft,$(FONTS),$(ft).h) fonts.h

clean:
	rm -f $(OBJS) $(LIBFILE) fonts/*.o allfonts.h
	touch allfonts.h

render.o: render.c render.h display.h fonts.h

.PHONY: allfonts.h
allfonts.h:
	(echo "#include <lcd/fonts.h>";for a in $(FONTS) ; do echo "#include <lcd/$$a.h>"; done) > $@
