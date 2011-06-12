/***************************************************************
 *
 * OpenBeacon.org - SPI routines for LPC13xx
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
#ifndef __SPI_H__
#define __SPI_H__

typedef uint32_t spi_cs;

#define SPI_CS_MODE_NORMAL 0
#define SPI_CS_MODE_INVERT_CS 1
#define SPI_CS_MODE_BIT_REVERSED 2
#define SPI_CS_MODE_SKIP_TX 4
#define SPI_CS_MODE_SKIP_CS_ASSERT 8
#define SPI_CS_MODE_SKIP_CS_DEASSERT 0x10
#define SPI_CS_MODE_SKIP_CS (SPI_CS_MODE_SKIP_CS_ASSERT|SPI_CS_MODE_SKIP_CS_DEASSERT)
#define SPI_CS(port,pin,CPSDVSR,mode) ((spi_cs)( ((((uint32_t)port)&0xFF)<<24) | ((((uint32_t)pin)&0xFF)<<16) | ((((uint32_t)CPSDVSR)&0xFF)<<8) | (((uint32_t)mode)&0xFF) ))

extern void spi_init (void);
extern void spi_status (void);
extern void spi_init_pin (spi_cs chipselect);
extern int spi_txrx (spi_cs chipselect, const void *tx, uint16_t txlen,
		     void *rx, uint16_t rxlen);

#endif/*__SPI_H__*/
