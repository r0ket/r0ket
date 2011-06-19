/***************************************************************
 *
 * OpenBeacon.org - opcode & register definitions for nRF24L01
 *
 * Copyright 2007 Milosch Meriac <meriac@openbeacon.de>
 *
 * provides a nice set of defines to work properly with the
 * nRF24L01 CPU
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
#ifndef NRF_HW_H
#define NRF_HW_H

//********************************************************************************************************************//
// SPI(nRF24L01) commands
#define READ_REG        0x00	// Define read command to register
#define WRITE_REG       0x20	// Define write command to register
#define RD_RX_PLOAD     0x61	// Define RX payload register address
#define WR_TX_PLOAD     0xA0	// Define TX payload register address
#define FLUSH_TX        0xE1	// Define flush TX register command
#define FLUSH_RX        0xE2	// Define flush RX register command
#define REUSE_TX_PL     0xE3	// Define reuse TX payload register command
#define OP_NOP          0xFF	// Define No Operation, might be used to read status register
#define ACTIVATE        0x50	// ACTIVATE additional features
#define R_RX_PL_WID     0x60	// Define Read RX-payload width command
#define W_ACK_PAYLOAD   0xA8	// Write payload to be used in ACK packet on pipe PPP
#define W_TX_PAYLOAD_NOACK 0xB0	// Used in TX mode, Disable AUTOACK on this specific packet


//********************************************************************************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00	// 'Config' register address
#define EN_AA           0x01	// 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02	// 'Enabled RX addresses' register address
#define SETUP_AW        0x03	// 'Setup address width' register address
#define SETUP_RETR      0x04	// 'Setup Auto. Retrans' register address
#define RF_CH           0x05	// 'RF channel' register address
#define RF_SETUP        0x06	// 'RF setup' register address
#define STATUS          0x07	// 'Status' register address
#define OBSERVE_TX      0x08	// 'Observe TX' register address
#define CD              0x09	// 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A	// 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B	// 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C	// 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D	// 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E	// 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F	// 'RX address pipe5' register address
#define TX_ADDR         0x10	// 'TX address' register address
#define RX_PW_P0        0x11	// 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12	// 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13	// 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14	// 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15	// 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16	// 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17	// 'FIFO Status Register' register address
#define FEATURE         0x1D	// Additional features register, needed to enable the additional commands

//********************************************************************************************************************//
// SPI(nRF24L01) registers(bitmasks)
#define ERX_P0		0x01	// Enable Pipe 0 (register EN_RXADDR)
#define ERX_P1		0x02	// Enable Pipe 1 (register EN_RXADDR)
#define ERX_P2		0x04	// Enable Pipe 2 (register EN_RXADDR)
#define ERX_P3		0x08	// Enable Pipe 3 (register EN_RXADDR)
#define ERX_P4		0x10	// Enable Pipe 4 (register EN_RXADDR)
#define ERX_P5		0x20	// Enable Pipe 5 (register EN_RXADDR)

//********************************************************************************************************************//
// 'Config' register mask bits
#define NRF_CONFIG_PRIM_RX      0x01
#define NRF_CONFIG_PWR_UP       0x02
#define NRF_CONFIG_CRCO         0x04
#define NRF_CONFIG_EN_CRC       0x08
#define NRF_CONFIG_MASK_MAX_RT  0x10
#define NRF_CONFIG_MASK_TX_DS   0x20
#define NRF_CONFIG_MASK_RX_DR   0x40
#define NRF_CONFIG_MASK_IRQS    (NRF_CONFIG_MASK_MAX_RT|NRF_CONFIG_MASK_TX_DS|NRF_CONFIG_MASK_RX_DR)


#define MASK_RX_DR_FLAG		0x40
#define MASK_TX_DS_FLAG		0x20
#define MASK_MAX_RT_FLAG	0x10
#define MASK_IRQ_FLAGS		(MASK_MAX_RT_FLAG|MASK_TX_DS_FLAG|MASK_RX_DR_FLAG)

#define FIFO_RX_EMPTY		0x01
#define FIFO_RX_FULL		0x02
#define FIFO_TX_EMPTY		0x10
#define FIFO_TX_FULL		0x20
#define FIFO_TX_REUSE		0x40

#define NRF_MIN_MAC_SIZE 3
#define NRF_MAX_MAC_SIZE 5
#define NRF_MAX_BUFFER_SIZE 32

#endif /*NRF_HW_H */
