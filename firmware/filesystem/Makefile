##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =

OBJS += ff.o
OBJS += diskio.o
OBJS += iobase.o
OBJS += mmc.o
OBJS += at45db041d.o
OBJS += util.o
OBJS += select.o
OBJS += execute.o

LIBNAME=fat

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH?= ..
INCLUDE_PATHS = -I$(ROOT_PATH) -I../core -I.

include $(ROOT_PATH)/Makefile.inc

##########################################################################
# Actual work
##########################################################################

include $(ROOT_PATH)/Makefile.util
