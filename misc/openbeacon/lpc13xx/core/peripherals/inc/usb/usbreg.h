/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    USBREG.H
 *      Purpose: USB Hardware Layer Definitions for NXP LPC13xx
 *      Version: V1.20
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
 *---------------------------------------------------------------------------*/

#ifndef __USBREG_H
#define __USBREG_H

/* Device Interrupt Bit Definitions */
#define FRAME_INT           (0x1<<0)
#define EP0_INT             (0x1<<1)
#define EP1_INT             (0x1<<2)
#define EP2_INT             (0x1<<3)
#define EP3_INT             (0x1<<4)
#define EP4_INT             (0x1<<5)
#define EP5_INT             (0x1<<6)
#define EP6_INT             (0x1<<7)
#define EP7_INT             (0x1<<8)
#define DEV_STAT_INT        (0x1<<9)
#define CCEMTY_INT          (0x1<<10)
#define CDFULL_INT          (0x1<<11)
#define RxENDPKT_INT        (0x1<<12)
#define TxENDPKT_INT        (0x1<<13)

/* Rx & Tx Packet Length Definitions */
#define PKT_LNGTH_MASK      0x000003FF
#define PKT_DV              0x00000400
#define PKT_RDY             0x00000800

/* USB Control Definitions */
#define CTRL_RD_EN          0x00000001
#define CTRL_WR_EN          0x00000002

/* Command Codes */
#define CMD_SET_ADDR        0x00D00500
#define CMD_CFG_DEV         0x00D80500
#define CMD_SET_MODE        0x00F30500
#define CMD_RD_INT          0x00F40500
#define DAT_RD_INT          0x00F40200
#define CMD_RD_FRAME        0x00F50500
#define DAT_RD_FRAME        0x00F50200
#define CMD_RD_CHIP_ID      0x00FD0500
#define DAT_RD_CHIP_ID      0x00FD0200

#define CMD_SET_DEV_STAT    0x00FE0500
#define CMD_GET_DEV_STAT    0x00FE0500
#define DAT_GET_DEV_STAT    0x00FE0200
#define CMD_GET_ERR_CODE    0x00FF0500
#define DAT_GET_ERR_CODE    0x00FF0200

#define DAT_WR_BYTE(x)     (0x00000100 | ((x) << 16))
#define CMD_SEL_EP(x)      (0x00000500 | ((x) << 16))
#define DAT_SEL_EP(x)      (0x00000200 | ((x) << 16))
#define CMD_SEL_EP_CLRI(x) (0x00400500 | ((x) << 16))
#define DAT_SEL_EP_CLRI(x) (0x00400200 | ((x) << 16))
#define CMD_SET_EP_STAT(x) (0x00400500 | ((x) << 16))
#define CMD_CLR_BUF         0x00F20500
#define CMD_VALID_BUF       0x00FA0500

/* Device Address Register Definitions */
#define DEV_ADDR_MASK       0x7F
#define DEV_EN              0x80

/* Device Configure Register Definitions */
#define CONF_DVICE          0x01

/* Device Mode Register Definitions */
#define AP_CLK              0x01
#define INAK_CI             0x02
#define INAK_CO             0x04
#define INAK_AI             0x08
#define INAK_AO             0x10

/* Device Status Register Definitions */
#define DEV_CON             0x01
#define DEV_CON_CH          0x02
#define DEV_SUS             0x04
#define DEV_SUS_CH          0x08
#define DEV_RST             0x10

/* Error Code Register Definitions */
#define ERR_EC_MASK         0x0F
#define ERR_EA              0x10

/* Error Status Register Definitions */
#define ERR_NOERROR			0x00
#define ERR_PID_ENCODE      0x01
#define ERR_UNKNOWN_PID     0x02
#define ERR_UNEXPECT_PKT    0x03
#define ERR_TCRC            0x04
#define ERR_DCRC            0x05
#define ERR_TIMEOUT         0x06
#define ERR_BABBIE          0x07
#define ERR_EOF_PKT         0x08
#define ERR_TX_RX_NAK		0x09
#define ERR_SENT_STALL      0x0A
#define ERR_BUF_OVERRUN     0x0B
#define ERR_SENT_EPT_PKT    0x0C
#define ERR_BIT_STUFF       0x0D
#define ERR_SYNC            0x0E
#define ERR_TOGGLE_BIT      0x0F

/* Endpoint Select Register Definitions */
#define EP_SEL_F            0x01
#define EP_SEL_ST           0x02
#define EP_SEL_STP          0x04
#define EP_SEL_PO           0x08
#define EP_SEL_EPN          0x10
#define EP_SEL_B_1_FULL     0x20
#define EP_SEL_B_2_FULL     0x40

/* Endpoint Status Register Definitions */
#define EP_STAT_ST          0x01
#define EP_STAT_DA          0x20
#define EP_STAT_RF_MO       0x40
#define EP_STAT_CND_ST      0x80

/* Clear Buffer Register Definitions */
#define CLR_BUF_PO          0x01

#endif /* __USBREG_H */
