project (qsimulat0r)
cmake_minimum_required(VERSION 2.4.0)

if(POLICY CMP0015)
   cmake_policy(SET CMP0015 OLD)
endif(POLICY CMP0015)

find_package(Qt4 REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../firmware ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/core)
link_directories(${CMAKE_CURRENT_BINARY_DIR}
../../firmware/applications 
../../firmware/filesystem
../../firmware/lcd 
../../firmware/funk 
../../firmware/basic 
../../firmware/core 
../../firmware/usbcdc 
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
../simcore/timecounter.o
../firmware/table.o
)


SET_SOURCE_FILES_PROPERTIES(${FIRMWARE_OBJS} PROPERTIES
                               GENERATED TRUE
                               EXTERNAL_OBJECT TRUE
			       OBJECT_DEPENDS TRUE
)

add_executable(qsimulat0r ${qsimulat0r_SRCS} ${MocSources}
 ${FIRMWARE_OBJS}
)

target_link_libraries(qsimulat0r
${QT_LIBRARIES}
libapp.a
liblcd.a
libusb.a
libfat.a
libfunk.a
libusbcdc.a
libbasic.a
libcore.a

liblcd.a

)



