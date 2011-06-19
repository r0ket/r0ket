TARGET=usbcdc-storage
ARCH=LPC13
CPU=$(ARCH)43
DEBUG=-g
OPTIM=-Os -mword-relocations

APP_CFLAGS=-Iinc -std=gnu99 -fgnu89-inline -D__USE_CMSIS
APP_LDFLAGS=-lm

APP_SRC= \
  src/main.c \
  src/cdcuser.c \
  src/clkconfig.c \
  src/DiskImg.c \
  src/mscuser.c \
  src/usbcore.c \
  src/compusbdesc.c \
  src/usbhw.c \
  src/usbuser.c

APP_SRC+=$(IMAGES_C)

all: $(TARGET).bin

app_clean:
	find src -name '*.o' -exec rm \{\} \;

include ../core/Makefile.rules
