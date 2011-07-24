

CFLAGS += -std=gnu99
CFLAGS += -I../firmware
CFLAGS += -I../firmware/core # for gpio.h including projectconfig.h without path
CFLAGS += -I../simcore

OBJS = simcore.o misc.o 

.PHONY : all clean
all : $(OBJS)

clean:
	$(RM) $(OBJS)
