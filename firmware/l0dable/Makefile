##########################################################################
# User configuration and firmware specific object files	
##########################################################################
SRCS = $(wildcard *.c)
OBJS = $(foreach mod,$(SRCS),$(subst .c,.o,$(mod)))
ELFS = $(foreach mod,$(SRCS),$(subst .c,.elf,$(mod)))
BINS = $(foreach mod,$(SRCS),$(subst .c,.bin,$(mod)))

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH?= ..

INCLUDE_PATHS = -I$(ROOT_PATH) -I$(ROOT_PATH)/core

include $(ROOT_PATH)/Makefile.inc

##########################################################################
# Compiler settings, parameters and flags
##########################################################################
LDSRCFILE=ram.ld
LDFILE=loadable.ld
CFLAGS+=-mlong-calls -fno-toplevel-reorder

all: $(OBJS) $(ELFS) $(BINS)

$(LDFILE):
	-@echo "MEMORY" > $(LDFILE)
	-@echo "{" >> $(LDFILE)
	-@echo "    sram(rwx): ORIGIN = 0x10002000 - $(RAMCODE), LENGTH = $(RAMCODE)" >> $(LDFILE)
	-@echo "}" >> $(LDFILE)
	-@echo "INCLUDE $(LDSRCFILE)" >> $(LDFILE)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

%.elf: %.o $(FIRMWARE) $(LDFILE)
	$(LD) $(LDFLAGS) -T $(LDFILE) -o $@ $<
	$(SIZE) $@

%.bin: %.elf
	$(OBJCOPY) $(OCFLAGS) -O binary $< $@

clean:
	rm -f *.o *.elf *.bin usetable.h

$(OBJS): usetable.h

usetable.h:
	./mktable.pl

.SUFFIXES:

.PHONY: $(LDFILE)
