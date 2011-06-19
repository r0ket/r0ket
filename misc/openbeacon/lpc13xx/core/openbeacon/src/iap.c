/***************************************************************
 *
 * OpenBeacon.org - IAP - in application programming
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
#include "iap.h"

typedef void (*IAP_FUNC) (const uint32_t * cmd, uint32_t * result);

static const IAP_FUNC iap = (IAP_FUNC) 0x1fff1ff1;

void
iap_invoke_isp (void)
{
  static const uint32_t cmd = 57;

  (*iap) (&cmd, NULL);
}

void
iap_read_uid (TDeviceUID * uid)
{
  static const uint32_t cmd = 58;

  (*iap) (&cmd, (uint32_t *) uid);
}
