/***************************************************************
 *
 * OpenBeacon.org - PN532 routines for LPC13xx based OpenPCD2
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
#include "pn532.h"
#include "rfid.h"

#define BIT_REVERSE(x) ((unsigned char)(__RBIT(x)>>24))

static void rfid_reset(unsigned char reset)
{
	GPIOSetValue(PN532_RESET_PORT, PN532_RESET_PIN, reset ? 1 : 0);
}

static void rfid_cs(unsigned char cs)
{
	GPIOSetValue(PN532_CS_PORT, PN532_CS_PIN, cs ? 1 : 0);
}

static unsigned char rfid_tx(unsigned char data)
{
	while ((LPC_SSP->SR & 0x02) == 0);
	LPC_SSP->DR = BIT_REVERSE (data);
	while ((LPC_SSP->SR & 0x04) == 0);
	data = BIT_REVERSE (LPC_SSP->DR);
	return data;
}

static unsigned char rfid_rx(void)
{
	return rfid_tx(0x00);
}

static unsigned char rfid_status(void)
{
	unsigned char res;

	/* enable chip select */
	rfid_cs(0);

	/* transmit status request */
	rfid_tx(0x02);
	res = rfid_rx();

	/* release chip select */
	rfid_cs(1);

	return res;
}

int rfid_read(void *data, unsigned char size)
{
	int res;
	unsigned char *p, c, pkt_size, crc, prev, t;

	/* wait 100ms max till PN532 response is ready */
	t=0;
	while ((rfid_status() & 1) == 0)
	{
		if(t++>10)
			return -8;
		vTaskDelay( 10 / portTICK_RATE_MS);
	}

	/* enable chip select */
	rfid_cs(0);

	/* read from FIFO command */
	rfid_tx(0x03);

	/* default result */
	res = -1;

	/* find preamble */
	t = 0;
	prev = rfid_rx();
	while ((!(((c = rfid_rx()) == 0xFF) && (prev == 0x00))) && (t
			< PN532_FIFO_SIZE))
	{
		prev = c;
		t++;
	}

	if (t >= PN532_FIFO_SIZE)
		res = -3;
	else
	{
		/* read packet size */
		pkt_size = rfid_rx();

		/* special treatment for NACK and ACK */
		if ((pkt_size == 0x00) || (pkt_size == 0xFF))
		{
			/* verify if second length byte is inverted */
			if (rfid_rx() != (unsigned char) (~pkt_size))
				res = -2;
			else
			{
				/* eat Postamble */
				rfid_rx();
				/* -1 for NACK, 0 for ACK */
				res = pkt_size ? 0 : -1;
			}
		}
		else
		{
			/* verify packet size against LCS */
			if (((pkt_size + rfid_rx()) & 0xFF) != 0)
				res = -4;
			else
			{
				/* remove TFI from packet size */
				pkt_size--;
				/* verify if packet fits into buffer */
				if (pkt_size > size)
					res = -5;
				else
				{
					/* remember actual packet size */
					size = pkt_size;
					/* verify TFI */
					if ((crc = rfid_rx()) != 0xD5)
						res = -6;
					else
					{
						/* read packet */
						p = (unsigned char *) data;
						while (pkt_size--)
						{
							/* read data */
							c = rfid_rx();
							/* maintain crc */
							crc += c;
							/* save payload */
							if (p)
								*p++ = c;
						}

						/* add DCS to CRC */
						crc += rfid_rx();
						/* verify CRC */
						if (crc)
							res = -7;
						else
						{
							/* eat Postamble */
							rfid_rx();
							/* return actual size as result */
							res = size;
						}
					}
				}
			}
		}
	}
	rfid_cs(1);

	/* everything fine */
	return res;
}

int rfid_write(const void *data, int len)
{
	int i;
	static const unsigned char preamble[] =
	{ 0x01, 0x00, 0x00, 0xFF };
	const unsigned char *p = preamble;
	unsigned char tfi = 0xD4, c;

	if (!data)
		len = 0xFF;

	/* enable chip select */
	rfid_cs(0);

	p = preamble; /* Praeamble */
	for (i = 0; i < (int) sizeof(preamble); i++)
		rfid_tx(*p++);
	rfid_tx(len + 1); /* LEN */
	rfid_tx(0x100 - (len + 1)); /* LCS */
	rfid_tx(tfi); /* TFI */
	/* PDn */
	p = (const unsigned char *) data;
	while (len--)
	{
		c = *p++;
		rfid_tx(c);
		tfi += c;
	}
	rfid_tx(0x100 - tfi); /* DCS */
	rfid_rx(); /* Postamble */

	/* release chip select */
	rfid_cs(1);

	/* check for ack */
	return rfid_read(NULL, 0);
}

static void rfid_hexdump(const void *buffer, int size)
{
	int i;
	const unsigned char *p = (unsigned char *) buffer;

	for (i = 0; i < size; i++)
	{
		if (i && ((i & 3) == 0))
			debug_printf(" ");
		debug_printf(" %02X", *p++);
	}
	debug_printf(" [size=%02i]\n", size);
}

static int rfid_execute(void *data, unsigned int isize, unsigned int osize)
{
	int res;
	if (rfid_write(data, isize))
	{
		debug_printf("getting result\n");
		res = rfid_read(data, osize);
		if (res > 0)
			rfid_hexdump(data, res);
		else
			debug_printf("error: res=%i\n", res);
	}
	else
	{
		debug_printf("->NACK!\n");
		res = -1;
	}
	return res;
}

void WriteRegister(unsigned short address, unsigned char data)
{
	unsigned char cmd[4];

	/* write register */
	cmd[0] = 0x08;
	/* high byte of address */
	cmd[1] = address >> 8;
	/* low byte of address */
	cmd[2] = address & 0xFF;
	/* data value */
	cmd[3] = data;

	rfid_execute(&cmd, sizeof(cmd), sizeof(data));
}

static
void rfid_task(void *pvParameters)
{
	int i;
	static unsigned char data[80];

	/* touch unused Parameter */
	(void) pvParameters;

	/* release reset line after 400ms */
	vTaskDelay( 400 / portTICK_RATE_MS);
	rfid_reset(1);
	/* wait for PN532 to boot */
	vTaskDelay( 100 / portTICK_RATE_MS);

	/* read firmware revision */
	debug_printf("\nreading firmware version...\n");
	data[0] = PN532_CMD_GetFirmwareVersion;
	rfid_execute(&data, 1, sizeof(data));

	/* enable debug output */
	debug_printf("\nenabling debug output...\n");
	WriteRegister(0x6328, 0xFC);
	// select test bus signal
	WriteRegister(0x6321, 6);
	// select test bus type
	WriteRegister(0x6322, 0x07);

	while (1)
	{
		/* wait 500ms */
		vTaskDelay( 500 / portTICK_RATE_MS);

		/* detect cards in field */
		GPIOSetValue(LED_PORT, LED_BIT, LED_ON);
		debug_printf("\nchecking for cards...\n");
		data[0] = PN532_CMD_InListPassiveTarget;
		data[1] = 0x01; /* MaxTg - maximum cards    */
		data[2] = 0x00; /* BrTy - 106 kbps type A   */
		if (((i = rfid_execute(&data, 3, sizeof(data))) >= 11) && (data[1]
				== 0x01) && (data[2] == 0x01))
		{
			debug_printf("card id: ");
			rfid_hexdump(&data[7], data[6]);
		}
		else
			debug_printf("unknown response of %i bytes\n", i);
		GPIOSetValue(LED_PORT, LED_BIT, LED_OFF);

		/* turning field off */
		debug_printf("\nturning field off again...\n");
		data[0] = PN532_CMD_RFConfiguration;
		data[1] = 0x01; /* CfgItem = 0x01           */
		data[2] = 0x00; /* RF Field = off           */
		rfid_execute(&data, 3, sizeof(data));
	}
}

void rfid_init(void)
{
	/* reset SSP peripheral */
	LPC_SYSCON->PRESETCTRL = 0x01;

	/* Enable SSP clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 11);

	// Enable SSP peripheral
	LPC_IOCON->PIO0_8 = 0x01 | (0x01 << 3); /* MISO, Pulldown */
	LPC_IOCON->PIO0_9 = 0x01; /* MOSI */

	LPC_IOCON->SCKLOC = 0x00; /* route to PIO0_10 */
	LPC_IOCON->JTAG_TCK_PIO0_10 = 0x02; /* SCK */

	/* Set SSP clock to 4.5MHz */
	LPC_SYSCON->SSPCLKDIV = 0x01;
	LPC_SSP->CR0 = 0x0707;
	LPC_SSP->CR1 = 0x0002;
	LPC_SSP->CPSR = 0x02;

	/* Initialize chip select line */
	rfid_cs(1);
	GPIOSetDir(PN532_CS_PORT, PN532_CS_PIN, 1);

	/* Initialize RESET line */
	rfid_reset(0);
	GPIOSetDir(PN532_RESET_PORT, PN532_RESET_PIN, 1);

	/* Create PN532 task */
	xTaskCreate(rfid_task, (const signed char*) "RFID", TASK_RFID_STACK_SIZE,
			NULL, TASK_RFID_PRIORITY, NULL);
}
