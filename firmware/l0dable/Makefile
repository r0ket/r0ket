##########################################################################
# User configuration and firmware specific object files	
##########################################################################
SRCS = $(wildcard *.c)
OBJS = $(foreach mod,$(SRCS),$(subst .c,.o,$(mod)))
ELFS = $(foreach mod,$(SRCS),$(subst .c,.elf,$(mod)))
BINS = $(foreach mod,$(SRCS),$(subst .c,.bin,$(mod)))
CODS = $(foreach mod,$(SRCS),$(subst .c,.c0d,$(mod)))
NIKS = $(foreach mod,$(wildcard nick_*.c),$(subst .c,.nik,$(subst nick_,,$(mod))))

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

CRYPT=cp
CRYPTFLAGS=-p

all: $(OBJS) $(ELFS) $(BINS) $(CODS) $(NIKS) 1boot.int

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

%.c0d: %.bin
	$(CRYPT) $(CRYPTFLAGS) $< $@

%.nik: .PHONY
	@a=$@;a=nick_$${a%.nik}.c0d;echo mv $$a $@;mv $$a $@

1boot.int: 1boot.c0d .PHONY
	mv $< $@

clean:
	rm -f *.o *.elf *.bin usetable.h

$(OBJS): usetable.h

usetable.h:
	./mktable.pl

.SUFFIXES:

.PHONY: $(LDFILE)
