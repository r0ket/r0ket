VPATH = 
OBJS =
LIBS = 

##########################################################################
# Project-specific files 
##########################################################################

VPATH +=
OBJS += main.o

LIBS += lcd/liblcd.a
LIBS += basic/libbasic.a
LIBS += core/libcore.a
LIBS += applications/libapp.a
LIBS += filesystem/libfat.a
LIBS += usb/libusb.a
LIBS += funk/libfunk.a
LIBS += usbcdc/libusbcdc.a

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH = .
INCLUDE_PATHS = -I$(ROOT_PATH) -I$(ROOT_PATH)/core

include $(ROOT_PATH)/Makefile.inc

LDFLAGS+= -Wl,--gc-sections
VPATH += lpc1xxx
OBJS += $(TARGET)_handlers.o LPC1xxx_startup.o

##########################################################################
# Startup files
##########################################################################
LDLIBS  = -lm
LDLIBS += -Lapplications -lapp
LDLIBS += -Lfunk -lfunk
LDLIBS += -Lusbcdc -lusbcdc
LDLIBS += -Llcd -llcd
LDLIBS += -Lusb -lusb
LDLIBS += -Lfilesystem -lfat
LDLIBS += -Lbasic -lbasic
LDLIBS += -Lcore -lcore
OCFLAGS = --strip-unneeded

SUBDIRS?= $(foreach lib,$(LIBS),$(dir $(lib)))

LD_PATH = lpc1xxx
LD_SCRIPT = $(LD_PATH)/linkscript.ld
LD_TEMP = $(LD_PATH)/memory.ld

### User targets:

all: $(OUTFILE).bin

protect: $(OUTFILE).bin
	$(LPCFIX) -p 2 $(OUTFILE).bin

loadables: $(OUTFILE).bin
	@cd loadable && $(MAKE)

clean:
	rm -f $(OBJS) $(LD_TEMP) $(OUTFILE).elf $(OUTFILE).bin $(OUTFILE).hex
	for dir in $(SUBDIRS); do \
	    $(MAKE) --no-print-directory -C $$dir clean; \
	done
flash: all
	../tools/bootloader/autoflash

### Internal targets

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

### Make all libraries...
.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) ROOT_PATH=../$(ROOT_PATH) --no-print-directory -C $@

../tools/bootloader/lpcfix:
	cd ../tools/bootloader && $(MAKE) 

$(LD_TEMP):
	-@echo "MEMORY" > $(LD_TEMP)
	-@echo "{" >> $(LD_TEMP)
	-@echo "  flash(rx): ORIGIN = 0x00000000, LENGTH = $(FLASH)" >> $(LD_TEMP)
	-@echo "  sram(rwx): ORIGIN = 0x10000000+$(SRAM_USB), LENGTH = $(SRAM)-$(SRAM_USB)-$(RAMCODE)" >> $(LD_TEMP)
	-@echo "}" >> $(LD_TEMP)
	-@echo "INCLUDE $(LD_SCRIPT)" >> $(LD_TEMP)

$(OUTFILE).elf: $(OBJS) $(SYS_OBJS) $(SUBDIRS) $(LPCFIX) $(LD_TEMP)
	$(CC) $(LDFLAGS) -T $(LD_TEMP) -o $(OUTFILE).elf $(OBJS) $(LDLIBS)
	-@echo ""
	$(SIZE) $(OUTFILE).elf
	-@echo ""

%.bin: %.elf
	$(OBJCOPY) $(OCFLAGS) -O binary $< $@
	-@echo ""
	$(LPCFIX) -c $@

.PHONY: $(LD_TEMP)

