/***************************************************************
 *
 * OpenBeacon.org - midlevel access function defines for
 * issuing raw commands to the nRF24L01 2.4Ghz frontend
 *
 * Copyright 2007 Milosch Meriac <meriac@openbeacon.de>
 *
 * provides generic register level access functions
 * for accessing nRF24L01 registers at a generic level
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
#ifndef NRF_CMD_H
#define NRF_CMD_H

extern uint8_t nRFCMD_CmdExec (uint8_t reg);
extern uint8_t nRFCMD_RegRead (uint8_t reg);
extern uint8_t nRFCMD_RegWriteStatusRead (uint8_t reg, uint8_t value);
extern uint8_t nRFCMD_RegWriteBuf (uint8_t reg, const uint8_t * buf, uint8_t count);
extern uint8_t nRFCMD_RegReadBuf (uint8_t reg, uint8_t * buf, uint8_t count);
extern uint8_t nRFCMD_GetRegSize (uint8_t reg);
extern uint8_t nRFCMD_WaitRx (uint32_t ticks);
extern void nRFCMD_CE (uint8_t enable);
extern void nRFCMD_Power (uint8_t enable);
extern void nRFCMD_ReadWriteBuffer (const uint8_t * tx_data, uint8_t * rx_data, uint32_t len);
extern void nRFCMD_ExecMacro (const uint8_t * macro);
extern void nRFCMD_RegisterDump (void);
extern void nRFCMD_Init (void);
extern void nRFCMD_Status (void);

#endif /*NRF_CMD_H */
