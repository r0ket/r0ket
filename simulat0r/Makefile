all : tui gui

.PHONY : tui gui tui-core clean

tui-core :
	$(MAKE) -C ../firmware/l0dable usetable.h
	cd ..; $(SHELL) simulat0r/bin/update-bridge-files.sh
$(MAKE) -C firmware
	$(MAKE) -C simcore
	$(MAKE) -C tui

tui : tui-core
	$(MAKE) -C gui/build clean # workaround for buggy dependency of gui build on libapp

.IGNORE : tui

gui : tui gui/build/Makefile 
	$(MAKE) -C gui/build VERBOSE=1

# bootstrap build directory 
gui/build/Makefile : tui-core
	install -d gui/build && cd gui/build && cmake ..


clean:
	$(MAKE) -C firmware clean
	$(MAKE) -C tui clean
	$(MAKE) -C simcore clean
	$(MAKE) -C gui/build clean # workaround for buggy dependency of gui build on libapp
