/***************************************************************
 *
 * OpenBeacon.org - PN532 definitions for LPC13xx based OpenPCD2
 *
 * Copyright 2010 Milosch Meriac <meriac@openbeacon.de>
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

#ifndef __PN532_H__
#define __PN532_H__

/* PN532 Hardware Settings */
#define PN532_FIFO_SIZE 64
#define PN532_ACK_NACK_SIZE 6

/* PN532 Miscellaneous command set */
#define PN532_CMD_Diagnose 0x00
#define PN532_CMD_GetFirmwareVersion 0x02
#define PN532_CMD_GetGeneralStatus 0x04
#define PN532_CMD_ReadRegister 0x06
#define PN532_CMD_WriteRegister 0x08
#define PN532_CMD_ReadGPIO 0x0C
#define PN532_CMD_WriteGPIO 0x0E
#define PN532_CMD_SetSerialBaudRate 0x10
#define PN532_CMD_SetParameters 0x12
#define PN532_CMD_SAMConfiguration 0x14
#define PN532_CMD_PowerDown 0x16

/* PN532 RF Communication command set */
#define PN532_CMD_RFConfiguration 0x32
#define PN532_CMD_RFRegulationTest 0x58

/* PN532 as Initiator */
#define PN532_CMD_InJumpForDEP 0x56
#define PN532_CMD_InJumpForPSL 0x46
#define PN532_CMD_InListPassiveTarget 0x4A
#define PN532_CMD_InATR 0x50
#define PN532_CMD_InPSL 0x4E
#define PN532_CMD_InDataExchange 0x40
#define PN532_CMD_InCommunicateThru 0x42
#define PN532_CMD_InDeselect 0x44
#define PN532_CMD_InRelease 0x52
#define PN532_CMD_InSelect 0x54
#define PN532_CMD_InAutoPoll 0x60

/* PN532 as Target */
#define PN532_CMD_TgInitAsTarget 0x8C
#define PN532_CMD_TgSetGeneralBytes 0x92
#define PN532_CMD_TgGetData 0x86
#define PN532_CMD_TgSetData 0x8E
#define PN532_CMD_TgSetMetaData 0x94
#define PN532_CMD_TgGetInitiatorCommand 0x88
#define PN532_CMD_TgResponseToInitiator 0x90
#define PN532_CMD_TgGetTargetStatus 0x8A

#endif/*__PN532_H__*/
