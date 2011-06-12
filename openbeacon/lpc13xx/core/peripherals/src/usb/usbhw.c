/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbhw.c
 * Purpose: USB Hardware Layer Module for Philips LPC17xx
 * Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC microcontroller devices only. Nothing else 
 *      gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Added USB_ClearEPBuf
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/
#include <openbeacon.h>
#ifdef  ENALBLE_USB_FULLFEATURED

#include "cdcusb.h"
#include "usbcfg.h"
#include "usbreg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbuser.h"


/*    
 *    USB and IO Clock configuration only. 
 *    The same as call PeriClkIOInit(IOCON_USB); 
 *    The purpose is to reduce the code space for 
 *    overall USB project and reserve code space for 
 *    USB debugging.
 *    Parameters:      None
 *    Return Value:    None
 */
void
USBIOClkConfig (void)
{
  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);

  /* Enable AHB clock to the USB block. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 14);
  LPC_IOCON->PIO0_3 &= ~0x1F;
  LPC_IOCON->PIO0_3 |= 0x01;	/* Secondary function VBUS */
  LPC_IOCON->PIO0_6 &= ~0x07;
  LPC_IOCON->PIO0_6 |= 0x01;	/* Secondary function SoftConn */
  return;
}

/*
 *  Delay number of clock cycles
 *    Parameters:      Delay length
 *    Return Value:    None
 */

void
delay (uint32_t length)
{
  volatile uint32_t i;

  for (i = 0; i < length; i++);
  return;
}

/*
 *  Get Endpoint Physical Address
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    Endpoint Physical Address
 */

uint32_t
EPAdr (uint32_t EPNum)
{
  uint32_t val;

  val = (EPNum & 0x0F) << 1;
  if (EPNum & 0x80)
    {
      val += 1;
    }
  return (val);
}


/*
 *  Write Command
 *    Parameters:      cmd:   Command
 *    Return Value:    None
 */

void
WrCmd (uint32_t cmd)
{

  LPC_USB->DevIntClr = CCEMTY_INT;
  LPC_USB->CmdCode = cmd;
  while ((LPC_USB->DevIntSt & (CCEMTY_INT | DEV_STAT_INT)) == 0);
}


/*
 *  Write Command Data
 *    Parameters:      cmd:   Command
 *                     val:   Data
 *    Return Value:    None
 */

void
WrCmdDat (uint32_t cmd, uint32_t val)
{

  WrCmd (cmd);
  WrCmd (val);
}


/*
 *  Write Command to Endpoint
 *    Parameters:      cmd:   Command
 *                     val:   Data
 *    Return Value:    None
 */

void
WrCmdEP (uint32_t EPNum, uint32_t cmd)
{

  WrCmd (CMD_SEL_EP (EPAdr (EPNum)));
  WrCmd (cmd);
}


/*
 *  Read Command Data
 *    Parameters:      cmd:   Command
 *    Return Value:    Data Value
 */

uint32_t
RdCmdDat (uint32_t cmd)
{

  LPC_USB->DevIntClr = CCEMTY_INT | CDFULL_INT;
  LPC_USB->CmdCode = cmd;
  while ((LPC_USB->DevIntSt & (CDFULL_INT | DEV_STAT_INT)) == 0);
  return (LPC_USB->CmdData);
}


/*
 *  USB Initialize Function
 *   Called by the User to initialize USB
 *    Return Value:    None
 */

void
USB_Init (void)
{

#if USB_FIQ_EVENT
  /* It's important that only BULK and FRAME(ISO) can be routed
     to FIQ. */
  LPC_USB->DevFIQSel = 0x01;	/* SOF Use FIQ */

  /* Enable the USB Interrupt */
  NVIC_EnableIRQ (USB_FIQn);
#endif

  /* Enable the USB Interrupt */
  NVIC_EnableIRQ (USB_IRQn);

  USB_Reset ();
  USB_SetAddress (0);
  return;
}


/*
 *  USB Connect Function
 *   Called by the User to Connect/Disconnect USB
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

void
USB_Connect (uint32_t con)
{
  WrCmdDat (CMD_SET_DEV_STAT, DAT_WR_BYTE (con ? DEV_CON : 0));
}


/*
 *  USB Reset Function
 *   Called automatically on USB Reset
 *    Return Value:    None
 */

void
USB_Reset (void)
{

  LPC_USB->DevIntClr = 0x000FFFFF;
  /* Enable all eight(8) EPs, note: EP won't be ready until it's
     configured/enabled when device sending SetEPStatus command 
     to the command engine. */
  LPC_USB->DevIntEn = DEV_STAT_INT | (0xFF << 1) |
    (USB_SOF_EVENT ? FRAME_INT : 0);
  return;
}


/*
 *  USB Suspend Function
 *   Called automatically on USB Suspend
 *    Return Value:    None
 */

void
USB_Suspend (void)
{
  /* Performed by Hardware */
}


/*
 *  USB Resume Function
 *   Called automatically on USB Resume
 *    Return Value:    None
 */

void
USB_Resume (void)
{
  /* Performed by Hardware */
}


/*
 *  USB Remote Wakeup Function
 *   Called automatically on USB Remote Wakeup
 *    Return Value:    None
 */

void
USB_WakeUp (void)
{

  if (USB_DeviceStatus & USB_GETSTATUS_REMOTE_WAKEUP)
    {
      WrCmdDat (CMD_SET_DEV_STAT, DAT_WR_BYTE (DEV_CON));
    }
}


/*
 *  USB Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Enable/Disable
 *    Return Value:    None
 */

void
USB_WakeUpCfg (uint32_t cfg)
{
  cfg = cfg;			/* Not needed */
}


/*
 *  USB Set Address Function
 *    Parameters:      adr:   USB Address
 *    Return Value:    None
 */

void
USB_SetAddress (uint32_t adr)
{
  WrCmdDat (CMD_SET_ADDR, DAT_WR_BYTE (DEV_EN | adr));	/* Don't wait for next */
  WrCmdDat (CMD_SET_ADDR, DAT_WR_BYTE (DEV_EN | adr));	/*  Setup Status Phase */
}


/*
 *  USB Configure Function
 *    Parameters:      cfg:   Configure/Deconfigure
 *    Return Value:    None
 */

void
USB_Configure (uint32_t cfg)
{

  WrCmdDat (CMD_CFG_DEV, DAT_WR_BYTE (cfg ? CONF_DVICE : 0));
  return;
}


/*
 *  Configure USB Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Endpoint Descriptor
 *    Return Value:    None
 */

void
USB_ConfigEP (USB_ENDPOINT_DESCRIPTOR * pEPD)
{
  (void) pEPD;
  return;
}


/*
 *  Set Direction for USB Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void
USB_DirCtrlEP (uint32_t dir)
{
  (void) dir;
}


/*
 *  Enable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void
USB_EnableEP (uint32_t EPNum)
{
  WrCmdDat (CMD_SET_EP_STAT (EPAdr (EPNum)), DAT_WR_BYTE (0));
}


/*
 *  Disable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void
USB_DisableEP (uint32_t EPNum)
{
  WrCmdDat (CMD_SET_EP_STAT (EPAdr (EPNum)), DAT_WR_BYTE (EP_STAT_DA));
}


/*
 *  Reset USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void
USB_ResetEP (uint32_t EPNum)
{
  WrCmdDat (CMD_SET_EP_STAT (EPAdr (EPNum)), DAT_WR_BYTE (0));
}


/*
 *  Set Stall for USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void
USB_SetStallEP (uint32_t EPNum)
{
  WrCmdDat (CMD_SET_EP_STAT (EPAdr (EPNum)), DAT_WR_BYTE (EP_STAT_ST));
}


/*
 *  Clear Stall for USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void
USB_ClrStallEP (uint32_t EPNum)
{
  WrCmdDat (CMD_SET_EP_STAT (EPAdr (EPNum)), DAT_WR_BYTE (0));
}


/*
 *  Clear USB Endpoint Buffer
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void
USB_ClearEPBuf (uint32_t EPNum)
{
  WrCmdEP (EPNum, CMD_CLR_BUF);
}


/*
 *  Read USB Endpoint Size
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    Number of bytes read
 */

uint32_t
USB_ReadEP_Count (uint32_t EPNum)
{
  uint32_t cnt;

  LPC_USB->Ctrl = ((EPNum & 0x0F) << 2) | CTRL_RD_EN;
  /* 3 clock cycles to fetch the packet length from RAM. */
  delay (5);

  while(((cnt = LPC_USB->RxPLen) & PKT_DV) == 0);

  return cnt & PKT_LNGTH_MASK;
}


/*
 *  Read USB Endpoint Data: Finalize Read 
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    Number of bytes read
 *
 *
 */

void
USB_ReadEP_Terminate (uint32_t EPNum)
{
  LPC_USB->Ctrl = 0;

  WrCmdEP (EPNum, CMD_CLR_BUF);
}


/*
 *  Read USB Endpoint Data
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *    Return Value:    Number of bytes read
 */

uint32_t
USB_ReadEP (uint32_t EPNum, uint8_t * pData)
{
  uint32_t res, cnt, data;
  uint8_t* p;

  res = cnt = USB_ReadEP_Count (EPNum);
  
  while(cnt>=sizeof(uint32_t))
  {
	*((uint32_t __attribute__ ((packed)) *) pData) = USB_ReadEP_Block ();
  	pData += sizeof(uint32_t);
  	cnt-=sizeof(uint32_t);
  }
  
  if(cnt>0)
  {
  	data = USB_ReadEP_Block ();
  	p = (uint8_t*)&data;
  	while(cnt--)
  		*pData++ = *p++;
  }
  
  USB_ReadEP_Terminate (EPNum);

  return res;
}


void
USB_WriteEP_Count (uint32_t EPNum, uint32_t cnt)
{
  LPC_USB->Ctrl = ((EPNum & 0x0F) << 2) | CTRL_WR_EN;
  /* 3 clock cycles to fetch the packet length from RAM. */
  delay (5);
  LPC_USB->TxPLen = cnt;
}


void
USB_WriteEP_Terminate (uint32_t EPNum)
{
  LPC_USB->Ctrl = 0;

  WrCmdEP (EPNum, CMD_VALID_BUF);
}


/*
 *  Write USB Endpoint Data
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *                     cnt:   Number of bytes to write
 *    Return Value:    Number of bytes written
 */

uint32_t
USB_WriteEP (uint32_t EPNum, uint8_t * pData, uint32_t cnt)
{
  uint32_t n;

  USB_WriteEP_Count (EPNum, cnt);

  for (n = 0; n < (cnt + 3) / 4; n++)
    {
	  USB_WriteEP_Block (*((uint32_t __attribute__ ((packed)) *) pData));
      pData += sizeof(uint32_t);
    }

  USB_WriteEP_Terminate (EPNum);

  return cnt;
}

/*
 *  Get USB Last Frame Number
 *    Parameters:      None
 *    Return Value:    Frame Number
 */

uint32_t
USB_GetFrame (void)
{
  uint32_t val;

  WrCmd (CMD_RD_FRAME);
  val = RdCmdDat (DAT_RD_FRAME);
  val = val | (RdCmdDat (DAT_RD_FRAME) << 8);

  return (val);
}


/*
 *  USB Interrupt Service Routine
 */

void
USB_IRQHandler (void)
{
  uint32_t disr, val, n, m;

  disr = LPC_USB->DevIntSt;	/* Device Interrupt Status */
  LPC_USB->DevIntClr = disr;

  /* Device Status Interrupt (Reset, Connect change, Suspend/Resume) */
  if (disr & DEV_STAT_INT)
    {
      WrCmd (CMD_GET_DEV_STAT);
      val = RdCmdDat (DAT_GET_DEV_STAT);	/* Device Status */
      if (val & DEV_RST)
	{			/* Reset */
	  USB_Reset ();
#if   USB_RESET_EVENT
	  USB_Reset_Event ();
#endif
	}
      if (val & DEV_CON_CH)
	{			/* Connect change */
#if   USB_POWER_EVENT
	  USB_Power_Event (val & DEV_CON);
#endif
	}
      if (val & DEV_SUS_CH)
	{			/* Suspend/Resume */
	  if (val & DEV_SUS)
	    {			/* Suspend */
	      USB_Suspend ();
#if     USB_SUSPEND_EVENT
	      USB_Suspend_Event ();
#endif
	    }
	  else
	    {			/* Resume */
	      USB_Resume ();
#if     USB_RESUME_EVENT
	      USB_Resume_Event ();
#endif
	    }
	}
      goto isr_end;
    }

#if USB_SOF_EVENT
  /* Start of Frame Interrupt */
  if (disr & FRAME_INT)
    {
      LPC_USB->DevIntClr = FRAME_INT;
      USB_SOF_Event ();
      SOFIRQCount++;
    }
#endif

#if USB_ERROR_EVENT
  /* NO error interrupt anymore, below code can be used
     as example to get error status from command engine. */
  /* Error Interrupt */
  if (disr & ERR_INT)
    {
      WrCmd (CMD_RD_ERR_STAT);
      val = RdCmdDat (DAT_RD_ERR_STAT);
      USB_Error_Event (val);
    }
#endif

  /* Endpoint's Interrupt */
  if (disr & (0xFF << 1))
    {
      /* if any of the EP0 through EP7 is set, or bit 1 through 9 on disr */
      for (n = 0; n < USB_EP_NUM; n++)
	{			/* Check All Endpoints */
	  /* skip frame interrupt at bit 0 in disr */
//      if (disr & ((1 << n)<<1)) {
	  if ((disr >> 1) & (1 << n))
	    {
	      m = n >> 1;
	      /* clear EP interrupt by sending cmd to the command engine. */
	      WrCmd (CMD_SEL_EP_CLRI (n));
	      val = RdCmdDat (DAT_SEL_EP_CLRI (n));
	      if ((n & 1) == 0)
		{		/* OUT Endpoint */
		  if (n == 0)
		    {		/* Control OUT Endpoint */
		      if (val & EP_SEL_STP)
			{	/* Setup Packet */
			  if (USB_P_EP[0])
			    {
			      USB_P_EP[0] (USB_EVT_SETUP);
			      continue;
			    }
			}
		    }
		  if (USB_P_EP[m])
		    {
		      USB_P_EP[m] (USB_EVT_OUT);
		    }
		}
	      else
		{		/* IN Endpoint */
		  if (USB_P_EP[m])
		    {
		      USB_P_EP[m] (USB_EVT_IN);
		    }
		}
	    }
	}
    }
isr_end:
  return;
}

#endif /*ENALBLE_USB_FULLFEATURED*/
