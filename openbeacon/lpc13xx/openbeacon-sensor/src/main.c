/***************************************************************
 *
 * OpenBeacon.org - main file for OpenBeacon Sensor (CR123A)
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
#include "sound.h"
#include "xxtea.h"
#include "pmu.h"
#include "iap.h"
#include "spi.h"
#include "nRF_API.h"
#include "nRF_CMD.h"
#include "openbeacon-proto.h"

uint32_t g_sysahbclkctrl;

#define FIFO_DEPTH 10
typedef struct {
    int x,y,z;
} TFifoEntry;

#define MAINCLKSEL_IRC 0
#define MAINCLKSEL_SYSPLL_IN 1
#define MAINCLKSEL_WDT 2
#define MAINCLKSEL_SYSPLL_OUT 3

/* device UUID */
static uint16_t tag_id;
static TDeviceUID device_uuid;

/* OpenBeacon packet */
static TBeaconEnvelope g_Beacon;

/* Default TEA encryption key of the tag - MUST CHANGE ! */
static const uint32_t xxtea_key[4] = { 0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF };

/* set nRF24L01 broadcast mac */
static const unsigned char broadcast_mac[NRF_MAX_MAC_SIZE] = { 1, 2, 3, 2, 1 };

static void
nRF_tx (uint8_t power)
{
  /* encrypt data */
  xxtea_encode(g_Beacon.block, XXTEA_BLOCK_COUNT, xxtea_key);

  /* set TX power */
  nRFAPI_SetTxPower (power & 0x3);

  /* upload data to nRF24L01 */
  nRFAPI_TX ((uint8_t*)&g_Beacon, sizeof(g_Beacon));

  /* transmit data */
  nRFCMD_CE (1);

  /* wait for packet to be transmitted */
  pmu_sleep_ms (10);

  /* transmit data */
  nRFCMD_CE (0);
}

void
nrf_off (void)
{
  /* disable RX mode */
  nRFCMD_CE (0);

  /* wait till RX is done */
  pmu_sleep_ms (5);

  /* switch to TX mode */
  nRFAPI_SetRxMode (0);

}

int
main (void)
{
  /* accelerometer readings fifo */
  TFifoEntry acc_lowpass;
  TFifoEntry fifo_buf[FIFO_DEPTH];
  int fifo_pos;
  TFifoEntry *fifo;
  uint32_t seq;

  volatile int i;
  int x, y, z, firstrun, tamper, moving;

  /* wait on boot - debounce */
  for (i = 0; i < 2000000; i++);

  /* Initialize GPIO (sets up clock) */
  GPIOInit ();

  /* initialize power management */
  pmu_init ();

  /* NVIC is installed inside UARTInit file. */
  UARTInit (115200, 0);

  LPC_IOCON->PIO2_0 = 0;
  GPIOSetDir (2, 0, 1);		//OUT
  GPIOSetValue (2, 0, 0);

  LPC_IOCON->RESET_PIO0_0 = 0;
  GPIOSetDir (0, 0, 0);		//IN

  LPC_IOCON->PIO0_1 = 0;
  GPIOSetDir (0, 1, 0);		//IN

  LPC_IOCON->PIO1_8 = 0;
  GPIOSetDir (1, 8, 1);		//OUT
  GPIOSetValue (1, 8, 0);

  LPC_IOCON->PIO0_2 = 0;
  GPIOSetDir (0, 2, 1);		//OUT
  GPIOSetValue (0, 2, 0);

  LPC_IOCON->PIO0_3 = 0;
  GPIOSetDir (0, 3, 0);		//IN

  LPC_IOCON->PIO0_4 = 1 << 8;
  GPIOSetDir (0, 4, 1);		//OUT
  GPIOSetValue (0, 4, 1);

  LPC_IOCON->PIO0_5 = 1 << 8;
  GPIOSetDir (0, 5, 1);		//OUT
  GPIOSetValue (0, 5, 1);

  LPC_IOCON->PIO1_9 = 0;	//FIXME
  GPIOSetDir (1, 9, 1);		//OUT
  GPIOSetValue (1, 9, 0);

  LPC_IOCON->PIO0_6 = 0;
  GPIOSetDir (0, 6, 1);		//OUT
  GPIOSetValue (0, 6, 1);

  LPC_IOCON->PIO0_7 = 0;
  GPIOSetDir (0, 7, 1);		//OUT
  GPIOSetValue (0, 7, 0);

  /* select UART_TXD */
  LPC_IOCON->PIO1_7 = 1;

  LPC_IOCON->PIO1_6 = 0;
  GPIOSetDir (1, 6, 1);		//OUT
  GPIOSetValue (1, 6, 0);

  LPC_IOCON->PIO1_5 = 0;
  GPIOSetDir (1, 5, 1);		//OUT
  GPIOSetValue (1, 5, 0);

  LPC_IOCON->PIO3_2 = 0;	// FIXME
  GPIOSetDir (3, 2, 1);		//OUT
  GPIOSetValue (3, 2, 1);

  LPC_IOCON->PIO1_11 = 0x80;	//FIXME
  GPIOSetDir (1, 11, 1);	// OUT
  GPIOSetValue (1, 11, 0);

  LPC_IOCON->PIO1_4 = 0x80;
  GPIOSetDir (1, 4, 0);		// IN

  LPC_IOCON->ARM_SWDIO_PIO1_3 = 0x81;
  GPIOSetDir (1, 3, 1);		// OUT
  GPIOSetValue (1, 3, 0);

  LPC_IOCON->JTAG_nTRST_PIO1_2 = 0x81;
  GPIOSetDir (1, 2, 1);		// OUT
  GPIOSetValue (1, 2, 0);

  LPC_IOCON->JTAG_TDO_PIO1_1 = 0x81;
  GPIOSetDir (1, 1, 1);		// OUT
  GPIOSetValue (1, 1, 0);

  LPC_IOCON->JTAG_TMS_PIO1_0 = 0x81;
  GPIOSetDir (1, 0, 1);		// OUT
  GPIOSetValue (1, 0, 0);

  LPC_IOCON->JTAG_TDI_PIO0_11 = 0x81;
  GPIOSetDir (0, 11, 1);	// OUT
  GPIOSetValue (0, 11, 0);

  LPC_IOCON->PIO1_10 = 0x80;
  GPIOSetDir (1, 10, 1);	// OUT
  GPIOSetValue (1, 10, 1);

  LPC_IOCON->JTAG_TCK_PIO0_10 = 0x81;
  GPIOSetDir (0, 10, 1);	// OUT
  GPIOSetValue (0, 10, 0);

  LPC_IOCON->PIO0_9 = 0;
  GPIOSetDir (0, 9, 1);		// OUT
  GPIOSetValue (0, 9, 0);

  /* select MISO function for PIO0_8 */
  LPC_IOCON->PIO0_8 = 1;

  /* initialize SPI */
  spi_init ();

#ifdef	SOUND_ENABLE
  /* Init Speaker Output */
  snd_init ();
#endif /*SOUND_ENABLE */

  /* Init 3D acceleration sensor */
  acc_init (0);

  /* read device UUID */
  bzero (&device_uuid, sizeof (device_uuid));
  iap_read_uid (&device_uuid);
  tag_id = crc16 ((uint8_t *) & device_uuid, sizeof (device_uuid));

  /* Initialize OpenBeacon nRF24L01 interface */
  if (!nRFAPI_Init (81, broadcast_mac, sizeof (broadcast_mac), 0))
    for (;;)
      {
	GPIOSetValue (1, 3, 1);
	pmu_sleep_ms (100);
	GPIOSetValue (1, 3, 0);
	pmu_sleep_ms (400);
      }
  /* set tx power power to high */
  nRFCMD_Power (1);

  /* blink LED for 1s to show readyness */
  GPIOSetValue (1, 3, 1);
  pmu_sleep_ms (1000);
  GPIOSetValue (1, 3, 0);

  /* reset fifo */
  fifo_pos=0;
  bzero(&fifo_buf,sizeof(fifo_buf));
  bzero(&acc_lowpass,sizeof(acc_lowpass));

  seq = firstrun = tamper = moving = 0;
  while (1)
    {
      /* read acceleration sensor */
      nRFAPI_SetRxMode(0);
      acc_power (1);
      pmu_sleep_ms (20);
      acc_xyz_read (&x, &y, &z);
      acc_power (0);

      /* prepare packet */
      bzero (&g_Beacon, sizeof (g_Beacon));
      g_Beacon.pkt.proto = RFBPROTO_BEACONTRACKER;
      g_Beacon.pkt.oid = htons (tag_id);
      g_Beacon.pkt.p.tracker.strength = 5;
      g_Beacon.pkt.p.tracker.seq = htonl (seq++);
      g_Beacon.pkt.p.tracker.reserved = moving;
      g_Beacon.pkt.crc = htons(crc16 (g_Beacon.byte, sizeof (g_Beacon) - sizeof (g_Beacon.pkt.crc)));
      /* transmit packet */
      nRF_tx (g_Beacon.pkt.p.tracker.strength);
      /* powering down */
      nRFAPI_PowerDown ();

      /* add new accelerometer values to lowpass */
      fifo = &fifo_buf[fifo_pos];
      if(fifo_pos>=(FIFO_DEPTH-1))
        fifo_pos=0;
      else
        fifo_pos++;

      acc_lowpass.x += x - fifo->x;
      fifo->x = x;
      acc_lowpass.y += y - fifo->y;
      fifo->y = y;
      acc_lowpass.z += z - fifo->z;
      fifo->z = z;

      if (!firstrun)
      {
        if(fifo_pos)
        {
	    pmu_sleep_ms (500);
	    continue;
	}
	else
	{
	    /* confirm finalized initialization by double-blink */
	    firstrun = 1;
	    GPIOSetValue (1, 3, 1);
	    pmu_sleep_ms (100);
	    GPIOSetValue (1, 3, 0);
	    pmu_sleep_ms (300);
	    GPIOSetValue (1, 3, 1);
	    pmu_sleep_ms (100);
	    GPIOSetValue (1, 3, 0);
	}
      }
      else
	if ((abs (acc_lowpass.x/FIFO_DEPTH - x) >= ACC_TRESHOLD) ||
	    (abs (acc_lowpass.y/FIFO_DEPTH - y) >= ACC_TRESHOLD) ||
	    (abs (acc_lowpass.z/FIFO_DEPTH - z) >= ACC_TRESHOLD))
	tamper = 5;

      if (tamper)
	{
	  pmu_sleep_ms (750);
	  tamper--;
	  if (moving < ACC_MOVING_TRESHOLD)
	    moving++;
	  else
	    {
	      snd_tone (22);
	      GPIOSetValue (1, 3, 1);
	      pmu_wait_ms (20);
	      GPIOSetValue (1, 3, 0);
	      snd_tone (23);
	      pmu_wait_ms (50);
	      snd_tone (24);
	      pmu_wait_ms (30);
	      snd_tone (0);
	    }
	}
      else
	{
	  pmu_sleep_ms (5000);
	  moving = 0;
	}
    }

  return 0;
}
