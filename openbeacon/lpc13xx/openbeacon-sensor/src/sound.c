/***************************************************************
 *
 * OpenBeacon.org - piezo speaker sound functions
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
#include <sound.h>

#ifdef  SOUND_ENABLE

#define ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

void
snd_beep (uint16_t frequency)
{
  uint32_t t;

  LPC_TMR32B1->TCR = 0;
  if (frequency)
    {
      t = (SYSTEM_CORE_CLOCK / 2) / frequency;

      LPC_TMR32B1->MR0 = LPC_TMR32B1->MR1 = t;

      if (LPC_TMR32B1->TC >= t)
	LPC_TMR32B1->TC = t;

      LPC_TMR32B1->TCR = 1;
    }
}

static inline uint16_t
snd_get_frequency_for_tone (uint8_t tone)
{
  static const uint16_t frequency[] = { 26263, 29366, 32963, 34923, 39200, 44000, 49388 };
  return (((uint32_t) frequency[tone % ARRAY_COUNT (frequency)]) * (1 << (tone/ARRAY_COUNT(frequency))))/100;
}

void
snd_tone (uint8_t tone)
{
  static uint8_t lasttone = 0;

  if (tone != lasttone)
    {
      if (tone)
	{
	  LPC_SYSCON->SYSAHBCLKCTRL |= EN_CT32B1;
	  LPC_TMR32B1->EMR = 1 | (0x3 << 4) | (0x3 << 6);
	  snd_beep (tone ? snd_get_frequency_for_tone (tone - 1) : 0);
	}
      else
	{
	  LPC_TMR32B1->EMR = 0;
	  LPC_SYSCON->SYSAHBCLKCTRL &= ~EN_CT32B1;
	}

      lasttone = tone;
    }
}

void
snd_init (void)
{
  /* Set sound port to PIO1_1 and PIO1_2 */
  LPC_GPIO1->DIR |= 0x6;
  LPC_IOCON->JTAG_TDO_PIO1_1 = 3;
  LPC_IOCON->JTAG_nTRST_PIO1_2 = 3;

  /* run 32 bit timer for sound generation */
  LPC_SYSCON->SYSAHBCLKCTRL |= EN_CT32B1;
  LPC_TMR32B1->TCR = 2;
  LPC_TMR32B1->MCR = 1 << 4;
  LPC_TMR32B1->EMR = 0;
}
#endif /*SOUND_ENABLE */
