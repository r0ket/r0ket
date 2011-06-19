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

#ifndef __SOUND_H__
#define __SOUND_H__

#define TONES_MAX 32

extern void snd_beep (uint16_t frequency);
extern void snd_tone (uint8_t tone);
extern void snd_init (void);

#endif/*__SOUND_H__*/
