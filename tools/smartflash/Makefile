setup:
	cd ../../firmware && make clean && make && make l0dables
	cp ../../firmware/firmware.bin .
	-mkdir files 1files
	cp ../../firmware/l0dable/*c0d 1files
	cp ../../firmware/l0dable/*nik files
	cp ../../firmware/l0dable/*int files
	cp ../../firmware/l0dable/files/* files
	cd ../crypto && make clean && make generate-keys
	cp ../crypto/generate-keys .
	@echo
	@echo Now run ./smartflash

clean:
	rm firmware.bin generate-keys
	rm -rf files 1files


