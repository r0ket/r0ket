/***************************************************************
 *
 * OpenBeacon.org - high level nRF24L01 access functions
 *
 * Copyright 2007 Milosch Meriac <meriac@openbeacon.de>
 *
 * provides high level initialization and startup sanity
 * checks and test routines to verify that the chip is working
 * properly and no soldering errors occored on the digital part.
 *
 ***************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/
#include <openbeacon.h>
#include "pmu.h"
#include "nRF_HW.h"
#include "nRF_CMD.h"
#include "nRF_API.h"

#ifndef NRF_RFOPTIONS
#define NRF_RFOPTIONS 0x09
#endif /*NRF_RFOPTIONS */

// set broadcast MAC to 'BCAST'
const uint8_t rfbroadcast_mac[NRF_MAX_MAC_SIZE] = { 'T', 'S', 'A', 'C', 'B' };

uint8_t
nRFAPI_DetectChip (void)
{
  uint8_t mac[NRF_MAX_MAC_SIZE], i;

  // blank read
  nRFAPI_GetStatus ();

  // set dummy MAC size
  nRFAPI_SetSizeMac (NRF_MIN_MAC_SIZE);

  // verify dummy MAC size
  if (nRFAPI_GetSizeMac () != NRF_MIN_MAC_SIZE)
    return 0;

  // set dummy MAC size
  nRFAPI_SetSizeMac (NRF_MAX_MAC_SIZE);

  // verify dummy MAC size
  if (nRFAPI_GetSizeMac () != NRF_MAX_MAC_SIZE)
    return 0;

  // set dummy MAC
  nRFAPI_SetTxMAC (rfbroadcast_mac, NRF_MAX_MAC_SIZE);

  //  get dummy MAC
  memset (&mac, 0, sizeof (mac));
  nRFAPI_GetTxMAC (mac, NRF_MAX_MAC_SIZE);

  // if can't verify written MAC - return with error
  for (i = 0; i < NRF_MAX_MAC_SIZE; i++)
    if (mac[i] != rfbroadcast_mac[i])
      return 0;

  // everything is fine
  return 1;
}

void
nRFAPI_SetRxMode (uint8_t receive)
{
  nRFCMD_RegWriteStatusRead (CONFIG | WRITE_REG, receive ? 0x3B : 0x3A);
}

void
nRFAPI_PowerDown (void)
{
  nRFCMD_RegWriteStatusRead (CONFIG | WRITE_REG, 0x00);
}

uint8_t
nRFAPI_Init (uint8_t channel,
	     const uint8_t * mac, uint8_t mac_size, uint8_t features)
{
  uint8_t i;

  // init IO layer of nRF24L01
  nRFCMD_Init ();

  /* wait for nRF to boot */
  pmu_sleep_ms(10);

  // check validity
  if (mac_size < 3 || mac_size > 5 || !nRFAPI_DetectChip ())
    return 0;

  // update mac
  nRFAPI_SetSizeMac (mac_size);
  nRFAPI_SetTxMAC (mac, mac_size);

  // enables pipe
  nRFAPI_SetRxMAC (mac, mac_size, 0);
  nRFAPI_PipesEnable (ERX_P0);
  nRFAPI_PipesAck (0);

  // set payload sizes
  for (i = 0; i <= 5; i++)
    nRFAPI_SetPipeSizeRX (i, 16);

  // set TX retry count
  nRFAPI_TxRetries (0);

  // set selected channel
  nRFAPI_SetChannel (channel);

  // set Tx power
  nRFAPI_SetTxPower (3);

  // flush FIFOs
  nRFAPI_FlushRX ();
  nRFAPI_FlushTX ();

  if (features != 0)
    nRFAPI_SetFeatures (features);

  return 1;
}

void
nRFAPI_SetTxPower (uint8_t power)
{
  if (power > 3)
    power = 3;

  nRFCMD_RegWriteStatusRead (RF_SETUP | WRITE_REG,
			     NRF_RFOPTIONS | (power << 1));
}

void
nRFAPI_TxRetries (uint8_t count)
{
  if (count > 15)
    count = 15;

  // setup delay of 500us+86us
  nRFCMD_RegWriteStatusRead (SETUP_RETR | WRITE_REG, 0x10 | count);
}

void
nRFAPI_PipesEnable (uint8_t mask)
{
  nRFCMD_RegWriteStatusRead (EN_RXADDR | WRITE_REG, mask & 0x3F);
}

void
nRFAPI_PipesAck (uint8_t mask)
{
  nRFCMD_RegWriteStatusRead (EN_AA | WRITE_REG, mask & 0x3F);
}

uint8_t
nRFAPI_GetSizeMac (void)
{
  uint8_t addr_size;

  addr_size = nRFCMD_RegRead (SETUP_AW) & 0x03;

  return addr_size ? addr_size + 2 : 0;
}

uint8_t
nRFAPI_SetSizeMac (uint8_t addr_size)
{
  if (addr_size >= 3 && addr_size <= 5)
    addr_size -= 2;
  else
    addr_size = 0;

  nRFCMD_RegWriteStatusRead (SETUP_AW | WRITE_REG, addr_size);

  return addr_size;
}

void
nRFAPI_GetTxMAC (uint8_t * addr, uint8_t addr_size)
{
  if (addr_size >= 3 && addr_size <= 5)
    nRFCMD_RegReadBuf (TX_ADDR, addr, addr_size);
}

void
nRFAPI_SetTxMAC (const uint8_t * addr, uint8_t addr_size)
{
  if (addr_size >= 3 && addr_size <= 5)
    nRFCMD_RegWriteBuf (TX_ADDR | WRITE_REG, addr, addr_size);
}

void
nRFAPI_SetRxMAC (const uint8_t * addr, uint8_t addr_size, uint8_t pipe)
{
  if ((pipe <= 1 && addr_size >= 3 && addr_size <= 5)
      || (addr_size == 1 && pipe >= 2 && pipe <= 5))
    nRFCMD_RegWriteBuf ((RX_ADDR_P0 + pipe) | WRITE_REG, addr, addr_size);
}

void
nRFAPI_SetChannel (uint8_t channel)
{
  nRFCMD_RegWriteStatusRead (RF_CH | WRITE_REG, channel & 0x7f);
}

uint8_t
nRFAPI_GetChannel (void)
{
  return nRFCMD_RegRead (RF_CH) & 0x7F;
}

uint8_t
nRFAPI_ClearIRQ (uint8_t status)
{
  return nRFCMD_RegWriteStatusRead (STATUS | WRITE_REG,
				    status & MASK_IRQ_FLAGS);
}

void
nRFAPI_TX (uint8_t * buf, uint8_t count)
{
  nRFCMD_RegWriteBuf (WR_TX_PLOAD, buf, count);
}

uint8_t
nRFAPI_GetStatus (void)
{
  return nRFCMD_CmdExec (OP_NOP);
}

uint8_t
nRFAPI_GetPipeSizeRX (uint8_t pipe)
{
  if (pipe <= 5)
    return nRFCMD_RegRead (RX_PW_P0 + pipe);
  else
    return 0;
}

void
nRFAPI_SetPipeSizeRX (uint8_t pipe, uint8_t size)
{
  if (pipe <= 5)
    nRFCMD_RegWriteStatusRead ((RX_PW_P0 + pipe) | WRITE_REG, size);
}

uint8_t
nRFAPI_GetPipeCurrent (void)
{
  return (nRFAPI_GetStatus () >> 1) & 0x7;
}

uint8_t
nRFAPI_RX (uint8_t * buf, uint8_t count)
{
  uint8_t size, pipe;

  pipe = nRFAPI_GetPipeCurrent ();
  if (pipe >= 7)
    size = 0;
  else
    {
      size = nRFAPI_GetPipeSizeRX (pipe);

      if (size <= count)
	nRFCMD_RegReadBuf (RD_RX_PLOAD, buf, size);
      else
	{
	  nRFAPI_FlushRX ();
	  size = 0;
	}
    }

  return size;
}

void
nRFAPI_FlushRX (void)
{
  nRFCMD_CmdExec (FLUSH_RX);
}

void
nRFAPI_FlushTX (void)
{
  nRFCMD_CmdExec (FLUSH_TX);
}

void
nRFAPI_ReuseTX (void)
{
  nRFCMD_CmdExec (REUSE_TX_PL);
}

uint8_t
nRFAPI_GetFifoStatus (void)
{
  return nRFCMD_RegRead (FIFO_STATUS);
}

uint8_t
nRFAPI_CarrierDetect (void)
{
  return nRFCMD_RegRead (CD);
}

void
nRFAPI_SetFeatures (uint8_t features)
{
  unsigned const char ACTIVATE_SEQUENCE[] = { ACTIVATE, 0x73 };
  uint8_t dummy_buffer[sizeof (ACTIVATE_SEQUENCE)] = { 0, 0 };
  nRFCMD_ReadWriteBuffer (ACTIVATE_SEQUENCE, dummy_buffer,
			  sizeof (ACTIVATE_SEQUENCE));
  nRFCMD_RegWriteStatusRead (FEATURE, features);
}
