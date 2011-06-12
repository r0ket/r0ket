/*****************************************************************************
 *   uart.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.21  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __UART_H__
#define __UART_H__
#ifndef UART_DISABLE

#define RS485_ENABLED	0
#define TX_INTERRUPT	0	/* 0 if TX uses polling, 1 interrupt driven. */
#define MODEM_TEST	0

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFSIZE		0x40

/* RS485 mode definition. */
#define RS485_NMMEN		(0x1<<0)
#define RS485_RXDIS		(0x1<<1)
#define RS485_AADEN		(0x1<<2)
#define RS485_SEL		(0x1<<3)
#define RS485_DCTRL		(0x1<<4)
#define RS485_OINV		(0x1<<5)

extern void UARTInit (uint32_t Baudrate,uint8_t rtscts);
extern void UART_IRQHandler (void);
extern BOOL UARTSendChar (uint8_t data);
extern void UARTSend (const uint8_t * BufferPtr, uint32_t Length);

extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];

#endif /* UART_DISABLE */
#endif /* end __UART_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/
