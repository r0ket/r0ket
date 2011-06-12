/***************************************************************
 *
 * OpenBeacon.org - 3D acceleration sensor support
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
#include <openbeacon.h>
#include "3d_acceleration.h"
#include "spi.h"

static void
acc_reg_write (uint8_t addr, uint8_t data)
{
  uint8_t tx[2];

  /* assemble SPI write request */
  tx[0] = 0x80 | addr << 1;
  tx[1] = data;

  /* transmit packet */
  spi_txrx (SPI_CS_ACC3D, tx, sizeof (tx), NULL, 0);
}

static uint8_t
acc_reg_read (uint8_t addr)
{
  uint8_t tx[2], rx[2];

  /* assemble SPI read request */
  tx[0] = addr << 1;
  tx[1] = 0;
  /* transmit packet */
  spi_txrx (SPI_CS_ACC3D, tx, sizeof (tx), rx, sizeof (rx));

  return rx[1];
}

void
acc_xyz_read (int *x, int *y, int *z)
{
  /* dummy read - FIXME */
  acc_reg_read (0);

  /*  get acceleration values */
  *x = (int8_t) acc_reg_read (6);
  *y = (int8_t) acc_reg_read (7);
  *z = (int8_t) acc_reg_read (8);
}

void
acc_status (void)
{
  int x, y, z;

  acc_xyz_read (&x, &y, &z);

  debug_printf (" * 3D_ACC: X=%04i Y=%04i Z=%04i\n", x, y, z);
}

void
acc_power (uint8_t enabled)
{
  /* dummy read - FIXME */
  acc_reg_read (0);
  /* set 3D acceleration sensor active, 2g - FIXME power saving */
  acc_reg_write (0x16, enabled ? (0x01 | 0x01 << 2) : 0x00);
}

void
acc_init (uint8_t enabled)
{
  /* PIO, PIO0_4 in standard IO functionality */
  LPC_IOCON->PIO0_4 = 1 << 8;

  /* setup SPI chipselect pin */
  spi_init_pin (SPI_CS_ACC3D);

  /* PIO, Inactive Pull, Digital Mode */
  LPC_IOCON->PIO1_11 = 0x80;
  GPIOSetDir (1, 11, 0);

  /* propagate power settings */  
  acc_power (enabled);
}
