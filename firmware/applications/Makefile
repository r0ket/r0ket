##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =

ifndef APP
APP=final
endif

ifeq "$(APP)" "initial"
NODEFAULT = y
endif

ifeq "$(APP)" "tester"
DEBUG = -d
endif

ifndef NODEFAULT
OBJS += default.o
endif

OBJS += $(foreach mod,$(APP),$(mod).o)

SRCS = $(foreach mod,$(APP),$(mod).c)

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

ifeq "$(FINAL)" "y"
CFLAGS+=-DFINAL
endif

##########################################################################
# Compiler settings, parameters and flags
##########################################################################

all: $(LIBFILE)

ifeq "$(APP)" "l0dable"
ifndef LAPP
LAPP=blinktest
endif
LSRC=../l0dable/$(LAPP).c
LOBJ=l0dable_$(LAPP).o

.PHONY: $(LOBJ)

$(LOBJ):
	$(CC) $(CFLAGS) -o $@ $(LSRC)

OBJS += $(LOBJ)
endif

ifeq "$(wildcard $(APP))" "$(APP)"
ifndef TYPE
TYPE=$(APP)
endif
TSRCS = $(foreach type,$(TYPE),$(wildcard $(type)/*.c))
TOBJS = $(foreach mod,$(TSRCS),$(subst .c,.o,$(mod)))

TWRAP=$(APP).gen

.PHONY: $(TWRAP)

$(APP).o: $(TWRAP)

$(TWRAP):
	./mkfirmware.pl $(DEBUG) $(APP) $(TSRCS) > $@

OBJS += $(TOBJS)
endif

$(LIBFILE): $(OBJS) $(WRAPOBJ)
	$(RM) -f $@
	$(AR) rcs $@ $(OBJS) $(WRAPOBJ)
	
%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(WRAPOBJ) $(WRAPSRC) $(LIBFILE) *.o */*.o *.gen

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
