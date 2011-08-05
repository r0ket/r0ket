##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =

OBJS += display.o
OBJS += render.o
OBJS += decoder.o
OBJS += backlight.o
OBJS += print.o
OBJS += image.o
OBJS += o.o

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

##########################################################################
# Actual work
##########################################################################

include $(ROOT_PATH)/Makefile.util

all: allfonts.h

$(FOBJS): $(foreach ft,$(FONTS),$(ft).h) fonts.h

clean::
	rm -f fonts/*.o allfonts.h
	touch allfonts.h

.PHONY: allfonts.h

allfonts.h:
	(echo "#include <lcd/fonts.h>";for a in $(FONTS) ; do echo "#include <lcd/$$a.h>"; done) > $@
