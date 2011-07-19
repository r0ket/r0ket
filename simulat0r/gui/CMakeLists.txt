project (qsimulat0r)
cmake_minimum_required(VERSION 2.4.0)
find_package(Qt4 REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../firmware ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/core)
link_directories(${CMAKE_CURRENT_BINARY_DIR}
../../firmware/applications 
../../firmware/filesystem
../../firmware/lcd 
../../firmware/usb)

include(${QT_USE_FILE})
set(qsimulat0r_SRCS
    qsimulat0r.cc
    simulat0rthread.cc
)

set(MocHeaders
    simulat0rthread.h
)

qt4_automoc(${MocHeaders})

QT_WRAP_CPP(qsimulat0r MocSources ${qsimulat0r_SRCS})


set(FIRMWARE_OBJS 
../simcore/simcore.o
../simcore/misc.o

../firmware/basic/basic.o
../firmware/basic/reinvoke_isp.o
../firmware/basic/delayms.o
../firmware/basic/uuid.o
../firmware/basic/keyin.o
../firmware/basic/voltage.o
../firmware/core/sysinit.o
../firmware/core/adc/adc.o
../firmware/core/cpu/cpu.o
../firmware/core/gpio/gpio.o
../firmware/core/i2c/i2c.o
../firmware/core/iap/iap.o
../firmware/core/libc/ctype.o
../firmware/core/libc/stdio.o
../firmware/core/libc/string.o
../firmware/core/pmu/pmu.o
../firmware/core/ssp/ssp.o
../firmware/core/systick/systick.o
../firmware/core/timer16/timer16.o
../firmware/core/timer32/timer32.o
../firmware/core/wdt/wdt.o
)


SET_SOURCE_FILES_PROPERTIES(${FIRMWARE_OBJS} PROPERTIES
                               GENERATED TRUE
                               EXTERNAL_OBJECT TRUE
			       OBJECT_DEPENDS TRUE
)

add_executable(qsimulat0r ${qsimulat0r_SRCS} ${MocSources}
 ${FIRMWARE_OBJS}
)

target_link_libraries(qsimulat0r ${QT_LIBRARIES} libapp.a liblcd.a libusb.a libfat.a)



