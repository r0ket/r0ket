usbcomp_msd_cdc
=====================
This project implements a USB composite class device that includes
a mass storage device and a virtual COM device.

To run this example, you must attach a USB cable to the board. See
the "Getting Started Guide" appendix for details. You may also
connect the LPCXpresso board to a base board from a 3rd party tool
partner.

When downloaded to the board and executed, the PC will first recognize
a USB mass storage device and mount a new drive that contains a text
file, and then it will recognize a USB "VCOM" device and ask for a
driver. Point windows to the .inf file in the usbcomp_msd_cdc project
directory to cause Windows to install the default USB serial driver.

At this point, you should be able to read/write files in the newly
mounted drive and send/receive characters through the USB virtual
COM port at the same time. The transmit baud rate will equal the 
CDC port configured baud rate.

One thing we have seen that can cause trouble with the USB examples
is the Windows driver install. Since the example projects all use
the same USB Vendor ID and Product ID, if you try out the HID
example and then try out the CDC example, Windows may try
to use the HID driver it had already installed with the CDC
example code. To fix this, go to the Windows Device Manager,
find the broken "HID" device and select "Uninstall." Then unplug the
device and plug it back in. Windows should correctly identify the
device as being a CDC device and install the correct
driver.

The project makes use of code from the following library projects:
- CMSISv1p30_LPC13xx : for CMSIS 1.30 files relevant to LPC13xx

This library project must exist in the same workspace in order
for the project to successfully build.

For more details, read the comments in config.h and
http://www.nxp.com/documents/application_note/AN11018.pdf
