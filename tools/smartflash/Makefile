default: build setup

all: build setup run

redo: rebuild setup run

build: cleanbuild rebuild

cleanbuild:
	cd ../../firmware && make clean

rebuild:
	cd ../../firmware && make && make l0dables

setup:
	cp ../../firmware/firmware.bin .
	mkdir -p files
	cp ../../firmware/l0dable/*c0d files
	cp ../../firmware/l0dable/*nik files
	cp ../../firmware/l0dable/*int files
	cp ../../firmware/l0dable/files/* files
	cp ../font/binary/*f0n files
	cp ../image/lcd/*lcd files
	cp ../image/lcd/i42.lcd files/nick.lcd
	cd ../crypto && make clean && make generate-keys
	cp ../crypto/generate-keys .
	@echo
	@echo "Now run ./smartflash (as root)"

run:
	sudo ./smartflash

clean:
	rm -f firmware.bin generate-keys
	rm -rf files
