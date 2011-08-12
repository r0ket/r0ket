ROOT_PATH=../firmware
include ../firmware/Makefile.inc

CFLAGS += -std=gnu99
CFLAGS += -I../firmware
CFLAGS += -I../firmware/core # for gpio.h including projectconfig.h without path
CFLAGS += -I../simcore

OBJS+= ../firmware/core/*.o
OBJS+= ../firmware/core/*/*.o
LIBS+= ../firmware/applications/libapp.a
LIBS+= ../firmware/lcd/liblcd.a
LIBS+= ../firmware/usb/libusb.a

#LIBS += -lm
LIBS += ../firmware/filesystem/libfat.a
LIBS += ../firmware/core/libcore.a
LIBS += ../firmware/funk/libfunk.a
LIBS += ../firmware/usbcdc/libusbcdc.a
LIBS += ../firmware/basic/libbasic.a



OBJS+=../simcore/simcore.o ../simcore/misc.o ../simcore/timecounter.o

OBJS += ../firmware/table.o

OBJS += simulat0r.o

.PHONY : all
all : simulat0r

simulat0r : $(OBJS) $(LIBS)

clean:
	$(RM) simulat0r.o
