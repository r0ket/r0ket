/***************************************************************
 *
 * OpenBeacon.org - function definitions
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
#ifndef NRF_API_H
#define NRF_API_H

#include <nRF_HW.h>

extern uint8_t nRFAPI_Init (uint8_t channel, const uint8_t * mac, uint8_t mac_size, uint8_t features);
extern void nRFAPI_SetTxPower (uint8_t power);
extern void nRFAPI_TxRetries (uint8_t count);
extern void nRFAPI_SetRxMode (uint8_t receive);
extern void nRFAPI_PipesEnable (uint8_t mask);
extern void nRFAPI_PipesAck (uint8_t mask);
extern uint8_t nRFAPI_GetSizeMac (void);
extern uint8_t nRFAPI_SetSizeMac (uint8_t addr_size);
extern void nRFAPI_GetTxMAC (uint8_t * addr, uint8_t addr_size);
extern void nRFAPI_SetTxMAC (const uint8_t * addr, uint8_t addr_size);
extern void nRFAPI_SetRxMAC (const uint8_t * addr, uint8_t addr_size, uint8_t pipe);
extern void nRFAPI_SetChannel (uint8_t channel);
extern uint8_t nRFAPI_GetChannel (void);
extern uint8_t nRFAPI_ClearIRQ (uint8_t status);
extern void nRFAPI_TX (uint8_t * buf, uint8_t count);
extern uint8_t nRFAPI_GetStatus (void);
extern uint8_t nRFAPI_GetPipeSizeRX (uint8_t pipe);
extern void nRFAPI_SetPipeSizeRX (uint8_t pipe, uint8_t size);
extern uint8_t nRFAPI_GetPipeCurrent (void);
extern uint8_t nRFAPI_RX (uint8_t * buf, uint8_t count);
extern void nRFAPI_FlushRX (void);
extern void nRFAPI_FlushTX (void);
extern void nRFAPI_ReuseTX (void);
extern uint8_t nRFAPI_GetFifoStatus (void);
extern uint8_t nRFAPI_CarrierDetect (void);
extern void nRFAPI_SetFeatures (uint8_t features);
extern void nRFAPI_PowerDown (void);

#endif /*NRF_API_H */
