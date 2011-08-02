#!/bin/sh
#
# test-all.sh
# Build all applications as firmware and simulat0r
# Results are saved as /tmp/firmware-appname.bin-buildtag or /tmp/qsimulat0r-...
# Call prefixed with BUILD="-myowntag" to replace default timestamp based build tag
# Setting BUILD empty suppresses the build tag
# Use TARGET=firmware or TARGET=simulat0r to limit compilation
# When building both targets and the firmware build failed the simulat0r build is skipped
#

if test ! -v BUILD; then
    BUILD=-`date --iso-8601=seconds`
    echo Setting BUILD to $BUILD
fi

if test ! -v TARGET; then
    BUILDFIRMWARE=1
    BUILDSIM=1
else
    case $TARGET in
	both |all ) BUILDFIRMWARE=1; BUILDSIM=1;;
	firmware | firm ) BUILDFIRMWARE=1; BUILDSIM=0;;
	simulat0r | simulator | sim ) BUILDFIRMWARE=0; BUILDSIM=1;;
	*) echo "Cannot build unknow target \"$TARGET\""; exit 1;;
    esac
fi

echo -n "Building "
if test $BUILDFIRMWARE -eq 1; then
    echo -n "firmware "
fi
if test $BUILDSIM -eq 1; then
    echo -n "simulat0r "
fi
echo

function doTest() 
{
    APP=$1
    FIRMWARESUCCESS=1
    
    if test $BUILDFIRMWARE -eq 1; then
	if make -C firmware APP=$APP 2>/dev/null >/dev/null
	then 
	    TMPFILE=/tmp/firmware-$APP.bin$BUILD
	    echo OK $APP as $TMPFILE
	    cp firmware/firmware.bin $TMPFILE
	else 
	    echo FIRMWARE BUILD FAILED $APP
	    FIRMWARESUCCESS=0
	fi
    fi
    
    if test $FIRMWARESUCCESS -eq 1 -a $BUILDSIM -eq 1; then
	if make -C simulat0r APP=$APP 2>/dev/null >/dev/null
	then 
	    TMPFILE=/tmp/qsimulat0r-$APP$BUILD
	    echo OK $APP as $TMPFILE
	    cp simulat0r/gui/build/qsimulat0r $TMPFILE
	else 
	    echo SIMULAT0R BUILD FAILED $APP
	fi
    fi
}

echo "Testing all applications..."

for i in `find firmware/applications/ -iname \*.c | sort` 
do
    if grep -q main_ $i
    then
	APP=`basename $i | sed "s/\..*//"`
	doTest $APP
    fi
done 
