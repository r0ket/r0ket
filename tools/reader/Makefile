ifneq "$(TUPLE)" ""
PREFIX=$(TUPLE)-
ARCH=$(shell echo $(TUPLE)|sed 's/-.*//')
else
PREFIX=
ARCH=$(shell uname -m)
endif

CC = $(PREFIX)gcc
STRIP = $(PREFIX)strip

CFLAGS = -Wall -O2
EXE = obreader
FILES = obreader.c

$(ARCH)-$(EXE): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)
	$(STRIP) $@


# Using OpenWRT crossbuild environment, see
# <URL:http://wiki.openwrt.org/doc/devel/crosscompile>
# for setup instructions.
WRT=~/r0ket/openwrt/trunk/staging_dir
STAGING_DIR=$(WRT)/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2

mips:
	STAGING_DIR=$(STAGING_DIR) \
	PATH=${PATH}:$(STAGING_DIR)/bin \
	$(MAKE) TUPLE=mips-openwrt-linux

clean: 
	rm -f *-$(EXE)
