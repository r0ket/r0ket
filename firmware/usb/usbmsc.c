#include "core/rom_drivers.h"
#include "core/gpio/gpio.h"
#include "filesystem/at45db041d.h"

#include "lcd/render.h"
#include "lcd/display.h"

#include "usb.h"
#include "usbconfig.h"
#include "usbmsc.h"

USB_DEV_INFO DeviceInfo;
MSC_DEVICE_INFO MscDevInfo;
ROM ** rom = (ROM **)0x1fff1ff8;
char usbMSCenabled=0;

void usbMSCWrite(uint32_t offset, uint8_t src[], uint32_t length) {
    dataflash_random_write(src, offset, length);
}

void usbMSCRead(uint32_t offset, uint8_t dst[], uint32_t length) {
    dataflash_random_read(dst, offset, length);
}

void usbMSCInit(void) {
  dataflash_initialize();

  // Setup USB clock
  SCB_PDRUNCFG &= ~(SCB_PDSLEEPCFG_USBPAD_PD);        // Power-up USB PHY
  SCB_PDRUNCFG &= ~(SCB_PDSLEEPCFG_USBPLL_PD);        // Power-up USB PLL

  SCB_USBPLLCLKSEL = SCB_USBPLLCLKSEL_SOURCE_MAINOSC; // Select PLL Input
  SCB_USBPLLCLKUEN = SCB_USBPLLCLKUEN_UPDATE;         // Update Clock Source
  SCB_USBPLLCLKUEN = SCB_USBPLLCLKUEN_DISABLE;        // Toggle Update Register
  SCB_USBPLLCLKUEN = SCB_USBPLLCLKUEN_UPDATE;

  // Wait until the USB clock is updated
  while (!(SCB_USBPLLCLKUEN & SCB_USBPLLCLKUEN_UPDATE));

  // Set USB clock to 48MHz (12MHz x 4)
  SCB_USBPLLCTRL = (SCB_USBPLLCTRL_MULT_4);
  while (!(SCB_USBPLLSTAT & SCB_USBPLLSTAT_LOCK));    // Wait Until PLL Locked
  SCB_USBCLKSEL = SCB_USBCLKSEL_SOURCE_USBPLLOUT;

  // Set USB pin functions
  // bsx says, its only needed for usb-hid. And it conflicts with btn_0
//  IOCON_PIO0_1 &= ~IOCON_PIO0_1_FUNC_MASK;
//  IOCON_PIO0_1 |= IOCON_PIO0_1_FUNC_CLKOUT;           // CLK OUT
  IOCON_PIO0_3 &= ~IOCON_PIO0_3_FUNC_MASK;
  IOCON_PIO0_3 |= IOCON_PIO0_3_FUNC_USB_VBUS;         // VBus
  IOCON_PIO0_6 &= ~IOCON_PIO0_6_FUNC_MASK;
  IOCON_PIO0_6 |= IOCON_PIO0_6_FUNC_USB_CONNECT;      // Soft Connect

  // Disable internal resistor on VBUS (0.3)
  gpioSetPullup(&IOCON_PIO0_3, gpioPullupMode_Inactive);

  // workaround for long connect delay
  *((uint32_t *)(0x10000054)) = 0x0;

  // HID Device Info
  volatile int n;
  MscDevInfo.idVendor = USB_VENDOR_ID;
  MscDevInfo.idProduct = USB_PROD_ID;
  MscDevInfo.bcdDevice = USB_DEVICE;
  MscDevInfo.StrDescPtr = (uint32_t)&USB_MSCStringDescriptor[0];
  MscDevInfo.MSCInquiryStr = (uint32_t)&"r0ket DataFlash             "; // 28 char response to SCSI INQUIRY
  MscDevInfo.BlockCount = 1024;
  MscDevInfo.BlockSize = 512;
  MscDevInfo.MemorySize = 1024*512;
  MscDevInfo.MSC_Write = &usbMSCWrite;
  MscDevInfo.MSC_Read = &usbMSCRead;

  DeviceInfo.DevType = USB_DEVICE_CLASS_STORAGE;
  DeviceInfo.DevDetailPtr = (uint32_t)&MscDevInfo;
  
  /* Enable Timer32_1, IOCON, and USB blocks (for USB ROM driver) */
  SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_CT32B1 | SCB_SYSAHBCLKCTRL_IOCON | SCB_SYSAHBCLKCTRL_USB_REG);

  /* Use pll and pin init function in rom */
  /* Warning: This will also set the system clock to 48MHz! */
  // (*rom)->pUSBD->init_clk_pins();   

  /* insert a delay between clk init and usb init */
  for (n = 0; n < 75; n++) {__asm("nop");}

  (*rom)->pUSBD->init(&DeviceInfo); /* USB Initialization */
  (*rom)->pUSBD->connect(true);     /* USB Connect */
  usbMSCenabled|=USB_MSC_ENABLEFLAG;
}

#if CFG_USBMSC
void USB_IRQHandler() {
  (*rom)->pUSBD->isr();
}
#endif

void usbMSCOff(void) {
  (*rom)->pUSBD->connect(false);     /* USB Disconnect */
  usbMSCenabled&=~USB_MSC_ENABLEFLAG;
}

