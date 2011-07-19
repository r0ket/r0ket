ROOT_PATH=../firmware
include ../firmware/Makefile.inc

CFLAGS += -std=gnu99
CFLAGS += -I../firmware
CFLAGS += -I../firmware/core # for gpio.h including projectconfig.h without path
CFLAGS += -I../simcore

OBJS+= ../firmware/basic/*.o
OBJS+= ../firmware/core/*.o
OBJS+= ../firmware/core/*/*.o
LDFLAGS+= -L../firmware/applications
LIBS+= ../firmware/applications/libapp.a
LDFLAGS+= -L../firmware/lcd
LIBS+= ../firmware/lcd/liblcd.a
LDFLAGS+= -L../firmware/usb
LIBS+= ../firmware/usb/libusb.a

#LIBS += -lm
LIBS += ../firmware/filesystem/libfat.a
LIBS += ../firmware/core/libcore.a


OBJS+=../simcore/simcore.o ../simcore/misc.o

.PHONY : all
all : simulat0r

simulat0r : simulat0r.o $(OBJS) $(LIBS) 

