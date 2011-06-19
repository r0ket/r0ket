drive=/cygdrive/f
file=$drive/firmware.bin

while : ; do
	while [ ! -f $file ] ; do
		sleep 1
	done
	cp firmware.bin $file
	echo -n '<Flashed>'
	echo -n '<Flashed>'|sed 's/.//g';
	while [ -f $file ] ; do
		sleep 1
	done
done

