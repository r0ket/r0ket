##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =
OBJS += cdcuser.o
OBJS += cdc_buf.o
OBJS += usbcore.o
OBJS += usbdesc.o
OBJS += usbhw.o
OBJS += usbuser.o
OBJS += util.o

LIBNAME=usbcdc

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
