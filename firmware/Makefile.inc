##########################################################################
# User configuration and firmware specific object files	
##########################################################################

# The target, flash and ram of the LPC1xxx microprocessor.
# Use for the target the value: LPC11xx, LPC13xx or LPC17xx
TARGET = LPC13xx
FLASH = 32K
SRAM = 8K
RAMCODE=2048

# For USB HID support the LPC134x reserves 384 bytes from the sram,
# if you don't want to use the USB features, just use 0 here.
SRAM_USB = 384

##########################################################################
# GNU GCC compiler prefix and location
##########################################################################

CROSS_COMPILE = arm-none-eabi-
AS = $(CROSS_COMPILE)gcc
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
REALLD = $(CROSS_COMPILE)ld
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
OUTFILE = firmware
LPCFIX = ../tools/bootloader/lpcfix

ifeq (LPC11xx,$(TARGET))
  CORTEX_TYPE=m0
else
  CORTEX_TYPE=m3
endif

CPU_TYPE = cortex-$(CORTEX_TYPE)

##########################################################################
# Compiler settings, parameters and flags
##########################################################################

CFLAGS  = -std=c99 -c -g -Os $(INCLUDE_PATHS) -Wall -mthumb -ffunction-sections -fdata-sections -fmessage-length=0 -mcpu=$(CPU_TYPE) -DTARGET=$(TARGET) -DRAMCODE=$(RAMCODE) -fno-builtin
LDFLAGS = -nostartfiles
