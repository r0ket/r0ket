##########################################################################
# User configuration and firmware specific object files	
##########################################################################

OBJS =
ifndef NODEFAULT
OBJS += default.o
endif

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

ifeq "$(APP)" "loadable"
ifndef LAPP
LAPP=blinktest
endif
LSRC=../loadable/$(LAPP).c
LOBJ=loadable_$(LAPP).o

.PHONY: $(LOBJ)

$(LOBJ):
	$(CC) $(CFLAGS) -o $@ $(LSRC)

OBJS += $(LOBJ)
endif

ifeq "$(APP)" "tester"
TSRCS = $(wildcard $(APP)/*.c)
TOBJS = $(foreach mod,$(TSRCS),$(subst .c,.o,$(mod)))

TWRAP=tester.gen

.PHONY: $(TWRAP)

$(APP).o: $(TWRAP)

$(TWRAP):
	./mktester $(TSRCS) > $@

OBJS += $(TOBJS)
endif

$(LIBFILE): $(OBJS) $(WRAPOBJ)
	$(RM) -f $@
	$(AR) rcs $@ $(OBJS) $(WRAPOBJ)
	
%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(WRAPOBJ) $(WRAPSRC) $(LIBFILE) *.o tester/*.o

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
