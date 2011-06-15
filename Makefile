VPATH = 
OBJS = main.o

##########################################################################
# Project-specific files 
##########################################################################

VPATH +=
OBJS +=
OBJS += basic/basic.o basic/reinvoke_isp.o basic/delayms.o basic/voltage.o
OBJS += basic/keyin.o
LIBS += core/libcore.a lcd/liblcd.a modules/libmodules.a filesystem/libfat.a

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
LDLIBS += -Lmodules -lmodules
LDLIBS += -Lfilesystem -lfat
LDLIBS += -Llcd -llcd
LDLIBS += -Lcore -lcore
OCFLAGS = --strip-unneeded

LD_PATH = lpc1xxx
LD_SCRIPT = $(LD_PATH)/linkscript.ld
LD_TEMP = $(LD_PATH)/memory.ld

### User targets:

all: $(OUTFILE).bin

protect: $(OUTFILE).bin
	$(LPCFIX) -p 2 $(OUTFILE).bin

loadables: $(OUTFILE).elf
	@cd loadable && $(MAKE)

clean:
	rm -f $(OBJS) $(LD_TEMP) $(OUTFILE).elf $(OUTFILE).bin $(OUTFILE).hex
	@cd core && $(MAKE) clean
	@cd tools && $(MAKE) clean
	@cd lcd && $(MAKE) clean
	@cd modules && $(MAKE) clean
	@cd filesystem && $(MAKE) clean
	@cd loadable && $(MAKE) clean

### Internal targets

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

core/libcore.a: core/projectconfig.h
	cd core && $(MAKE) ROOT_PATH=../$(ROOT_PATH)

lcd/liblcd.a lcd/render.o lcd/display.o:
	cd lcd && $(MAKE) ROOT_PATH=../$(ROOT_PATH)

modules/libmodules.a:
	cd modules && $(MAKE) ROOT_PATH=../$(ROOT_PATH)

filesystem/libfat.a:
	cd filesystem && $(MAKE) ROOT_PATH=../$(ROOT_PATH)

tools/lpcfix:
	cd tools && $(MAKE) 

$(LD_TEMP):
	-@echo "MEMORY" > $(LD_TEMP)
	-@echo "{" >> $(LD_TEMP)
	-@echo "  flash(rx): ORIGIN = 0x00000000, LENGTH = $(FLASH)" >> $(LD_TEMP)
	-@echo "  sram(rwx): ORIGIN = 0x10000000+$(SRAM_USB), LENGTH = $(SRAM)-$(SRAM_USB)-$(RAMCODE)" >> $(LD_TEMP)
	-@echo "}" >> $(LD_TEMP)
	-@echo "INCLUDE $(LD_SCRIPT)" >> $(LD_TEMP)

$(OUTFILE).elf: $(OBJS) $(SYS_OBJS) $(LIBS) $(LPCFIX) $(LD_TEMP)
	$(CC) $(LDFLAGS) -T $(LD_TEMP) -o $(OUTFILE).elf $(OBJS) $(LDLIBS)
	-@echo ""
	$(SIZE) $(OUTFILE).elf
	-@echo ""

%.bin: %.elf
	$(OBJCOPY) $(OCFLAGS) -O binary $< $@
	-@echo ""
	$(LPCFIX) -c $@

.PHONY: $(LD_TEMP) lcd/liblcd.a modules/libmodules.a filesystem/libfat.a

