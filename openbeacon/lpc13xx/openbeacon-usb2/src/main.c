/***************************************************************
 *
 * OpenBeacon.org - main file for OpenBeacon USB II Bluetooth
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
#include "pin.h"
#include "hid.h"
#include "spi.h"
#include "iap.h"
#include "pmu.h"
#include "crc16.h"
#include "xxtea.h"
#include "bluetooth.h"
#include "storage.h"
#include "nRF_API.h"
#include "nRF_CMD.h"
#include "openbeacon-proto.h"
#include "display.h"

/* OpenBeacon packet */
static TBeaconEnvelope g_Beacon;

/* Default TEA encryption key of the tag - MUST CHANGE ! */
static const uint32_t xxtea_key[4] =
//{ 0x00112233, 0x44556677, 0x8899AABB, 0xCCDDEEFF };
{ 0xb4595344, 0xd3e119b6, 0xa814d0ec, 0xeff5a24e };

/* set nRF24L01 broadcast mac */
static const unsigned char broadcast_mac[NRF_MAX_MAC_SIZE] =
{ 1, 2, 3, 2, 1 };

/* device UUID */
static uint16_t tag_id;
static TDeviceUID device_uuid;

#if (USB_HID_IN_REPORT_SIZE>0)||(USB_HID_OUT_REPORT_SIZE>0)
static uint8_t hid_buffer[USB_HID_IN_REPORT_SIZE];

void
GetInReport (uint8_t * src, uint32_t length)
{
	(void) src;
	(void) length;

	if (length > USB_HID_IN_REPORT_SIZE)
	length = USB_HID_IN_REPORT_SIZE;

	memcpy (src, hid_buffer, length);
}

void
SetOutReport (uint8_t * dst, uint32_t length)
{
	(void) dst;
	(void) length;
}
#endif

static void show_version(void)
{
	debug_printf(" * Tag ID: %i\n", (uint16_t) device_uuid[3]);
	debug_printf(" * Device UID: %08X:%08X:%08X:%08X\n", device_uuid[0],
			device_uuid[1], device_uuid[2], device_uuid[3]);
	debug_printf(" * free heap memory: %i bytes\n",xPortGetFreeHeapSize());
}

void main_menue(uint8_t cmd)
{
	/* ignore non-printable characters */
	if (cmd <= ' ')
		return;
	/* show key pressed */
	debug_printf("%c\n", cmd);
	/* map lower case to upper case */
	if (cmd > 'a' && cmd < 'z')
		cmd -= ('a' - 'A');

	switch (cmd)
	{
	case '?':
	case 'H':
		debug_printf("\n"
			" *****************************************************\n"
			" * OpenBeacon USB II - Bluetooth Console             *\n"
			" * (C) 2010 Milosch Meriac <meriac@openbeacon.de>    *\n"
			" *****************************************************\n"
			" * H,?          - this help screen\n"
#ifdef MENUE_ALLOW_ISP_REBOOT
				" * P            - invoke ISP programming mode\n"
#endif
				" * S            - show device status\n"
				" * R            - OpenBeacon nRF24L01 register dump\n"
				" *****************************************************\n"
				"\n");
		break;
#ifdef MENUE_ALLOW_ISP_REBOOT
		case 'P':
		debug_printf ("\nRebooting...");
		iap_invoke_isp ();
		break;
#endif
	case 'R':
		nRFCMD_RegisterDump();
		break;
	case 'S':
		debug_printf("\n"
			" *****************************************************\n"
			" * OpenBeacon Status Information                     *\n"
			" *****************************************************\n");
		show_version();
		spi_status();
		nRFCMD_Status();
		//      pmu_status ();
#if (DISK_SIZE>0)
		storage_status();
#endif
		nRFCMD_Status();
		debug_printf(" *****************************************************\n"
			"\n");
		break;
	default:
		debug_printf("Unknown command '%c' - please press 'H' for help \n", cmd);
	}
	debug_printf("\n# ");
}

static
void nRF_tx(uint8_t power)
{
	/* update crc */
	g_Beacon.pkt.crc = htons(crc16(g_Beacon.byte, sizeof(g_Beacon)
			- sizeof(uint16_t)));
	/* encrypt data */
	xxtea_encode(g_Beacon.block, XXTEA_BLOCK_COUNT, xxtea_key);

	pin_led(GPIO_LED0);

	/* update power pin */
	nRFCMD_Power(power & 0x4);

	/* disable RX mode */
	nRFCMD_CE(0);
	vTaskDelay(5 / portTICK_RATE_MS);

	/* switch to TX mode */
	nRFAPI_SetRxMode(0);

	/* set TX power */
	nRFAPI_SetTxPower(power & 0x3);

	/* upload data to nRF24L01 */
	nRFAPI_TX(g_Beacon.byte, sizeof(g_Beacon));

	/* transmit data */
	nRFCMD_CE(1);

	/* wait until packet is transmitted */
	vTaskDelay(2 / portTICK_RATE_MS);

	/* switch to RX mode again */
	nRFAPI_SetRxMode(1);

	pin_led(GPIO_LEDS_OFF);

	if (power & 0x4)
		nRFCMD_Power(0);
}

static
void nRF_Task(void *pvParameters)
{
	int t, active;
	uint8_t strength, status;
	uint16_t crc;
	uint32_t seq, oid;
	portTickType LastUpdateTicks, Ticks;

	(void) pvParameters;

	/* Initialize OpenBeacon nRF24L01 interface */
	if (!nRFAPI_Init(81, broadcast_mac, sizeof(broadcast_mac), 0))
		/* bail out if can't initialize */
		for (;;)
		{
			pin_led(GPIO_LED0 | GPIO_LED1);
			vTaskDelay(500 / portTICK_RATE_MS);

			pin_led(GPIO_LEDS_OFF);
			vTaskDelay(500 / portTICK_RATE_MS);
		}

	/* blink as a sign of boot to detect crashes */
	for (t = 0; t < 20; t++)
	{
		pin_led(GPIO_LED0);
		vTaskDelay(50 / portTICK_RATE_MS);

		pin_led(GPIO_LEDS_OFF);
		vTaskDelay(50 / portTICK_RATE_MS);
	}

	nRFAPI_SetRxMode(1);
	nRFCMD_CE(1);

	LastUpdateTicks = xTaskGetTickCount();

	/* main loop */
	active = 0;
	seq = t = 0;
	UARTCount = 0;
	while (1)
	{
		/* turn off after button press */
		if (!pin_button0())
		{
			pin_mode_pmu(0);
			pmu_off(0);
		}

		if (nRFCMD_WaitRx(10 / portTICK_RATE_MS))
			do
			{
				// read packet from nRF chip
				nRFCMD_RegReadBuf(RD_RX_PLOAD, g_Beacon.byte, sizeof(g_Beacon));

				// adjust byte order and decode
				xxtea_decode(g_Beacon.block, XXTEA_BLOCK_COUNT, xxtea_key);

				// verify the CRC checksum
				crc = crc16(g_Beacon.byte, sizeof(g_Beacon) - sizeof(uint16_t));

				if (ntohs (g_Beacon.pkt.crc) == crc)
				{
					pin_led(GPIO_LED1);

					oid = ntohs (g_Beacon.pkt.oid);
					if (((g_Beacon.pkt.flags & RFBFLAGS_SENSOR) > 0) && active)
						debug_printf("BUTTON: %i\n", oid);

					switch (g_Beacon.pkt.proto)
					{
					case RFBPROTO_READER_ANNOUNCE:
						strength = g_Beacon.pkt.p.reader_announce.strength;
						break;

					case RFBPROTO_BEACONTRACKER:
						strength = g_Beacon.pkt.p.tracker.strength;
						if (active)
							debug_printf(" R: %04i={%i,0x%08X}\n", (int) oid,
									(int) strength,
									ntohl (g_Beacon.pkt.p.tracker.seq));
						break;

					case RFBPROTO_PROXREPORT:
						strength = 3;
						if (active)
							debug_printf(" P: %04i={%i,0x%04X}\n", (int) oid,
									(int) strength,
									(int) ntohs (g_Beacon.pkt.p.prox.seq));
						for (t = 0; t < PROX_MAX; t++)
						{
							crc = (ntohs (g_Beacon.pkt.p.prox.oid_prox[t]));
							if (crc && active)
								debug_printf("PX: %04i={%04i,%i,%i}\n",
										(int) oid, (int) ((crc >> 0) & 0x7FF),
										(int) ((crc >> 14) & 0x3), (int) ((crc
												>> 11) & 0x7));
						}
						break;

					default:
						strength = 0xFF;
						if (active)
							debug_printf("Unknown Protocol: %i\n",
									(int) g_Beacon.pkt.proto);
					}

					if (strength < 0xFF)
					{
						/* do something with the data */
					}
					pin_led(GPIO_LEDS_OFF);
				}
				status = nRFAPI_GetFifoStatus();
			} while ((status & FIFO_RX_EMPTY) == 0);

		nRFAPI_ClearIRQ(MASK_IRQ_FLAGS);

		// update regularly
		if (((Ticks = xTaskGetTickCount()) - LastUpdateTicks)
				> UPDATE_INTERVAL_MS)
		{
			LastUpdateTicks = Ticks;

			/* setup tracking packet */
			bzero(&g_Beacon, sizeof(g_Beacon));
			g_Beacon.pkt.oid = ntohs ((uint16_t)device_uuid[3]);
			g_Beacon.pkt.proto = RFBPROTO_BEACONTRACKER;
			g_Beacon.pkt.p.tracker.strength = seq % 8;
			g_Beacon.pkt.p.tracker.seq = htonl(seq++);

			/* send away packet */
			nRF_tx(g_Beacon.pkt.p.tracker.strength);
		}

		if (UARTCount)
		{
			/* blink LED1 upon Bluetooth command */
			pin_led(GPIO_LED1);

			/* show help screen upon Bluetooth connect */
			if (!active)
			{
				active = 1;
				debug_printf("press 'H' for help...\n# ");
			}
			else
				/* execute menu command with last character received */
				main_menue(UARTBuffer[UARTCount - 1]);

			/* LED1 off again */
			pin_led(GPIO_LEDS_OFF);

			/* clear UART buffer */
			UARTCount = 0;
		}
	}
}

int main(void)
{
	volatile int i;
	/* wait on boot - debounce */
	for (i = 0; i < 2000000; i++)
		;

	//pmu_off(0);
    lcdInit();
	/* initialize  pins */
	pin_init();
	/* Init SPI */
	spi_init();
	/* Init Storage */
#ifdef USB_DISK_SUPPORT
	storage_init();
#endif
	/* Init USB HID interface */
#if (USB_HID_IN_REPORT_SIZE>0)||(USB_HID_OUT_REPORT_SIZE>0)
	hid_init ();
    #error hid
#endif
	/* power management init */
	pmu_init();
	/* read device UUID */
	bzero(&device_uuid, sizeof(device_uuid));
	iap_read_uid(&device_uuid);
	tag_id = crc16((uint8_t*) &device_uuid, sizeof(device_uuid));

	xTaskCreate(nRF_Task, (const signed char*) "nRF", TASK_NRF_STACK_SIZE,
			NULL, TASK_NRF_PRIORITY, NULL);

	/* Start the tasks running. */
	vTaskStartScheduler();

	return 0;
}
