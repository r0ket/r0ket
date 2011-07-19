##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS = default.o
OBJS += $(foreach mod,$(APP),$(mod).o)

SRCS = $(foreach mod,$(APP),$(mod).c)

ifndef APP
ME_OBJ=$(USERNAME)

ifeq "$(ME_OBJ)" ""
ME_OBJ=$(USER)
endif

ifeq "$(ME_OBJ)" ""
ME_OBJ=nouser
endif

OBJS += $(ME_OBJ).o
endif

ifeq "$(APP)" "loadable"
ifndef LAPP
LAPP=blinktest
endif
OBJS += ../loadable/$(LAPP).o
endif

WRAP=wrapper
LIBNAME=app

##########################################################################
# GNU GCC compiler flags
##########################################################################
ROOT_PATH?= ..
INCLUDE_PATHS = -I$(ROOT_PATH) -I../core -I.

include $(ROOT_PATH)/Makefile.inc

WRAPOBJ=$(WRAP).o
WRAPSRC=$(WRAP).c
LIBFILE=lib$(LIBNAME).a

##########################################################################
# Compiler settings, parameters and flags
##########################################################################

all: $(LIBFILE)

$(LIBFILE): $(OBJS) $(WRAPOBJ)
	$(AR) rcs $@ $(OBJS) $(WRAPOBJ)
	
%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(WRAPOBJ) $(WRAPSRC) $(LIBFILE) *.o

%.c:
	@echo 
	@echo "You need to create $@ first"
	@echo "It should contain a single function void main_filename(void)"
	@echo 
	@exit 1

$(WRAPSRC):
	./mkwrapper $(OBJS) > $@

.PHONY: $(LIBFILE) $(WRAPSRC) $(SRCS)

.SUFFIXES:
