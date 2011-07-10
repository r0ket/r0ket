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
OBJS += wdt/wdt.o

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH?= ..
INCLUDE_PATHS = -I$(ROOT_PATH) -I.

include $(ROOT_PATH)/Makefile.inc

##########################################################################
# Compiler settings, parameters and flags
##########################################################################

all: libcore.a

libcore.a: $(OBJS)
	$(AR) rcs libcore.a $(OBJS)
	
%.o : %.c projectconfig.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) libcore.a
