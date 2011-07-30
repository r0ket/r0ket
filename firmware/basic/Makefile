##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =

OBJS += basic.o
OBJS += reinvoke_isp.o
OBJS += delayms.o
OBJS += voltage.o
OBJS += night.o
OBJS += keyin.o
OBJS += uuid.o
OBJS += crc.o
OBJS += menu.o
OBJS += xxtea.o
OBJS += ecc.o
OBJS += byteorder.o
OBJS += random.o
OBJS += idle.o
OBJS += config.o
OBJS += itoa.o
OBJS += simpletime.o

LIBNAME=basic

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH?= ..
INCLUDE_PATHS = -I$(ROOT_PATH) -I../core -I.

include $(ROOT_PATH)/Makefile.inc

CFLAGS+=

##########################################################################
# Actual work
##########################################################################

include $(ROOT_PATH)/Makefile.util
