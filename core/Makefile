##########################################################################
# User configuration and firmware specific object files	
##########################################################################

# The target, flash and ram of the LPC1xxx microprocessor.
# Use for the target the value: LPC11xx, LPC13xx or LPC17xx
TARGET = LPC13xx

OBJS = sysinit.o
OBJS += adc/adc.o
#OBJS += cmd/cmd.o
OBJS += cpu/cpu.o
OBJS += gpio/gpio.o
OBJS += i2c/i2c.o
OBJS += iap/iap.o
OBJS += libc/ctype.o
OBJS += libc/stdio.o
OBJS += libc/string.o
OBJS += pmu/pmu.o
#OBJS += pwm/pwm.o
OBJS += ssp/ssp.o
OBJS += systick/systick.o
OBJS += timer16/timer16.o
OBJS += timer32/timer32.o
#OBJS += uart/uart.o
#OBJS += uart/uart_buf.o
#OBJS += usbcdc/cdcuser.o
#OBJS += usbcdc/cdc_buf.o
#OBJS += usbcdc/usbcore.o
#OBJS += usbcdc/usbdesc.o
#OBJS += usbcdc/usbhw.o
#OBJS += usbcdc/usbuser.o
#OBJS += usbhid-rom/usbconfig.o
#OBJS += usbhid-rom/usbhid.o
OBJS += wdt/wdt.o

##########################################################################
# GNU GCC compiler prefix and location
##########################################################################

CROSS_COMPILE = arm-none-eabi-
AS = $(CROSS_COMPILE)gcc
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
OUTFILE = firmware

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH = .
INCLUDE_PATHS = -I$(ROOT_PATH) -I..

##########################################################################
# Startup files
##########################################################################

ifeq (LPC11xx,$(TARGET))
  CORTEX_TYPE=m0
else
  CORTEX_TYPE=m3
endif

CPU_TYPE = cortex-$(CORTEX_TYPE)

##########################################################################
# Compiler settings, parameters and flags
##########################################################################

CFLAGS  = -c -g -Os $(INCLUDE_PATHS) -Wall -mthumb -ffunction-sections -fdata-sections -fmessage-length=0 -mcpu=$(CPU_TYPE) -DTARGET=$(TARGET) -fno-builtin
LDFLAGS = -nostartfiles -mthumb -mcpu=$(CPU_TYPE) -Wl,--gc-sections

all: libcore.a

libcore.a: $(OBJS)
	$(AR) rcs libcore.a $(OBJS)
	
%.o : %.c projectconfig.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) libcore.a
