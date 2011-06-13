VPATH = 
OBJS = main.o

##########################################################################
# Project-specific files 
##########################################################################

VPATH +=
OBJS +=
OBJS += basic/basic.o basic/reinvoke_isp.o basic/delayms.o basic/voltage.o
OBJS += basic/keyin.o
OBJS += eeprom/eeprom.o
LIBS += core/libcore.a lcd/liblcd.a modules/libmodules.a

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH = .
INCLUDE_PATHS = -I$(ROOT_PATH) -I$(ROOT_PATH)/core

include $(ROOT_PATH)/Makefile.inc

VPATH += lpc1xxx
OBJS += $(TARGET)_handlers.o LPC1xxx_startup.o

##########################################################################
# Startup files
##########################################################################
LDLIBS  = -lm
LDLIBS += -Lmodules -lmodules
LDLIBS += -Lcore -lcore
LDLIBS += -Llcd -llcd
OCFLAGS = --strip-unneeded

LD_PATH = lpc1xxx
LD_SCRIPT = $(LD_PATH)/linkscript.ld
LD_TEMP = $(LD_PATH)/memory.ld

all: $(OUTFILE).bin

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

core/libcore.a: core/projectconfig.h
	cd core && $(MAKE) ROOT_PATH=../$(ROOT_PATH)

lcd/liblcd.a lcd/render.o lcd/display.o:
	cd lcd && $(MAKE) ROOT_PATH=../$(ROOT_PATH)

modules/libmodules.a:
	cd modules && $(MAKE) ROOT_PATH=../$(ROOT_PATH)

tools/lpcfix:
	cd tools && $(MAKE) 

$(OUTFILE).bin: $(OBJS) $(SYS_OBJS) $(LIBS) $(LPCFIX)
	-@echo "MEMORY" > $(LD_TEMP)
	-@echo "{" >> $(LD_TEMP)
	-@echo "  flash(rx): ORIGIN = 0x00000000, LENGTH = $(FLASH)" >> $(LD_TEMP)
	-@echo "  sram(rwx): ORIGIN = 0x10000000+$(SRAM_USB), LENGTH = $(SRAM)-$(SRAM_USB)" >> $(LD_TEMP)
	-@echo "}" >> $(LD_TEMP)
	-@echo "INCLUDE $(LD_SCRIPT)" >> $(LD_TEMP)
	$(LD) $(LDFLAGS) -T $(LD_TEMP) -o $(OUTFILE).elf $(OBJS) $(LDLIBS)
	-@echo ""
	$(SIZE) $(OUTFILE).elf
	-@echo ""
	$(OBJCOPY) $(OCFLAGS) -O binary $(OUTFILE).elf $(OUTFILE).bin
	-@echo ""
	$(LPCFIX) -c $(OUTFILE).bin

protect: $(OUTFILE).bin
	$(LPCFIX) -p 2 $(OUTFILE).bin

clean:
	rm -f $(OBJS) $(LD_TEMP) $(OUTFILE).elf $(OUTFILE).bin $(OUTFILE).hex
	@cd core && $(MAKE) clean
	@cd tools && $(MAKE) clean
	@cd lcd && $(MAKE) clean
	@cd modules && $(MAKE) clean


.PHONY: lcd/liblcd.a modules/libmodules.a

