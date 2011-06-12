PROG = hid_listen

OS = LINUX
#OS = DARWIN
#OS = WINDOWS


ifeq ($(OS), LINUX)
TARGET = $(PROG)
CC = gcc
STRIP = strip
CFLAGS = -O2 -Wall -D$(OS)
LIBS =
else ifeq ($(OS), DARWIN)
TARGET = $(PROG)
CC = gcc
STRIP = strip
SDK = /Developer/SDKs/MacOSX10.5.sdk
CFLAGS = -O2 -Wall -isysroot $(SDK) -D$(OS) -arch ppc -arch i386
LIBS = -Xlinker -syslibroot -Xlinker $(SDK) -framework IOKit -framework CoreFoundation
else ifeq ($(OS), WINDOWS)
TARGET = $(PROG).exe
CC = i586-mingw32msvc-gcc
STRIP = i586-mingw32msvc-strip
WINDRES = i586-mingw32msvc-windres
CFLAGS =  -O2 -Wall -D$(OS)
LIBS = -lhid -lsetupapi
KEY_SPC = ~/bin/cert/mykey.spc
KEY_PVK = ~/bin/cert/mykey.pvk
KEY_TS = http://timestamp.comodoca.com/authenticode
endif


MAKEFLAGS = --jobs=2
OBJS = hid_listen.o rawhid.o

all: $(TARGET)

$(PROG): $(OBJS)
	gcc -o $(PROG) $(OBJS) $(LIBS)
	$(STRIP) $(PROG)

$(PROG).app: $(PROG) Info.plist
	mkdir -p $(PROG).app/Contents/MacOS
	mkdir -p $(PROG).app/Contents/Resources/English.lproj
	cp Info.plist $(PROG).app/Contents/
	echo -n 'APPL????' > $(PROG).app/Contents/PkgInfo
	cp $(PROG) $(PROG).app/Contents/MacOS/$(PROG)
	cp icons/$(PROG).icns $(PROG).app/Contents/Resources/$(PROG).icns
	touch $(PROG).app

$(PROG).dmg: $(PROG).app
	hdiutil create -ov -srcfolder $(PROG).app $(PROG).dmg

$(PROG).exe: $(OBJS)
	$(CC) $(OBJS) -o $(PROG).exe $(LIBS)
	$(STRIP) $(PROG).exe
	-signcode -spc $(KEY_SPC) -v $(KEY_PVK) -t $(KEY_TS) $(PROG).exe

resource.o: resource.rs icons/$(PROG).ico
	$(WINDRES) -o resource.o resource.rs



clean:
	rm -f *.o $(PROG) $(PROG).exe $(PROG).exe.bak $(PROG).dmg
	rm -rf $(PROG).app

