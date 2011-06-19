/*****************************************************************************
 *   rom_drivers.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.09.17  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef ROM_DRIVERS_H_
#define ROM_DRIVERS_H_

typedef struct _USB_DEVICE_INFO
{
  uint16_t DevType;
  uint32_t DevDetailPtr;
} USB_DEV_INFO;

typedef struct _USBD
{
  void (*init_clk_pins) (void);
  void (*isr) (void);
  void (*init) (const USB_DEV_INFO * DevInfoPtr);
  void (*connect) (uint32_t con);
} USBD;

#define init_msdstate() *((uint32_t *)(0x10000054)) = 0x0

typedef struct _ROM
{
  const USBD *pUSBD;
} ROM;

typedef struct _MSC_DEVICE_INFO
{
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint32_t StrDescPtr;
  uint32_t MSCInquiryStr;
  uint32_t BlockCount;
  uint32_t BlockSize;
  uint32_t MemorySize;
  void (*MSC_Write) (uint32_t offset, uint8_t *src, uint32_t length);
  void (*MSC_Read) (uint32_t offset, uint8_t *dst, uint32_t length);
} MSC_DEVICE_INFO;

typedef struct _HID_DEVICE_INFO
{
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint32_t StrDescPtr;
  uint8_t InReportCount;
  uint8_t OutReportCount;
  uint8_t SampleInterval;
  void (*InReport) (uint8_t *src, uint32_t length);
  void (*OutReport) (uint8_t *dst, uint32_t length);
} HID_DEVICE_INFO;

#endif /*ROM_DRIVERS_H_ */
