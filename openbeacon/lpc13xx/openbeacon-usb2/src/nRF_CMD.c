/***************************************************************
 *
 * OpenBeacon.org - midlevel access functions for
 * issuing raw commands to the nRF24L01 2.4Ghz frontend
 *
 * Copyright 2007 Milosch Meriac <meriac@openbeacon.de>
 *
 * provides generic register level access functions
 * for accessing nRF24L01 registers at a generic level
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
#include "nRF_HW.h"
#include "nRF_CMD.h"
#include "spi.h"

/* IO definitions */
#define RF_IRQ_CPU_PORT 0
#define RF_IRQ_CPU_PIN 7
#define CPU_CE_RF_PORT 1
#define CPU_CE_RF_PIN 5
//#define CPU_SWITCH_RF_PORT 0
//#define CPU_SWITCH_RF_PIN 2

#define SPI_MAX_XFER_LEN 33
#define NRFCMD_MACRO_READ 0x80

#define SPI_MAX_XFER_LEN 33
#define NRFCMD_MACRO_READ 0x80
static uint8_t spi_outbuf[SPI_MAX_XFER_LEN];
static uint8_t spi_inbuf[SPI_MAX_XFER_LEN];
static volatile uint8_t g_packet_rxed = 0;

static xSemaphoreHandle xnRF_SemaphoreACK;

void nRFCMD_CE(uint8_t enable)
{
	GPIOSetValue(CPU_CE_RF_PORT, CPU_CE_RF_PIN, enable ? 1 : 0);
}

void nRFCMD_Power(uint8_t enable)
{
    enable = 0;
	//GPIOSetValue(CPU_SWITCH_RF_PORT, CPU_SWITCH_RF_PIN, enable ? 0 : 1);
}

void nRFCMD_ReadWriteBuffer(const uint8_t * tx_data, uint8_t * rx_data,
		uint32_t len)
{
	spi_txrx(SPI_CS_NRF, tx_data, len, rx_data, len);
}

static uint8_t nRFCMD_ReadWriteByte(uint8_t reg)
{
	uint8_t res;

	nRFCMD_ReadWriteBuffer(&reg, &res, 1);

	return res;
}

uint8_t nRFCMD_CmdExec(uint8_t cmd)
{
	uint8_t res;

	res = nRFCMD_ReadWriteByte(cmd);

	return res;
}

uint8_t nRFCMD_RegRead(uint8_t reg)
{
	spi_outbuf[0] = reg;
	spi_outbuf[1] = 0;

	nRFCMD_ReadWriteBuffer(spi_outbuf, spi_inbuf, 2);

	return spi_inbuf[1];
}

uint8_t nRFCMD_RegWriteStatusRead(uint8_t reg, uint8_t value)
{
	spi_outbuf[0] = reg;
	spi_outbuf[1] = value;

	nRFCMD_ReadWriteBuffer(spi_outbuf, spi_inbuf, 2);

	return spi_inbuf[0];
}

uint8_t nRFCMD_RegWriteBuf(uint8_t reg, const uint8_t * buf, uint8_t count)
{
	spi_outbuf[0] = reg;
	memcpy(spi_outbuf + 1, buf, count);
	nRFCMD_ReadWriteBuffer(spi_outbuf, spi_inbuf, count + 1);

	return spi_inbuf[0];
}

uint8_t nRFCMD_RegReadBuf(uint8_t reg, uint8_t * buf, uint8_t count)
{
	spi_outbuf[0] = reg;
	nRFCMD_ReadWriteBuffer(spi_outbuf, spi_inbuf, count + 2);
	memcpy(buf, spi_inbuf + 1, count);

	return spi_inbuf[0];
}

uint8_t nRFCMD_GetRegSize(uint8_t reg)
{
	uint8_t res;

	if (reg > 0x17)
		res = 0;
	else
		switch (reg)
		{
		case RX_ADDR_P0:
		case RX_ADDR_P1:
		case TX_ADDR:
			res = NRF_MAX_MAC_SIZE;
			break;
		default:
			res = 1;
		}
	return res;
}

void nRFCMD_ExecMacro(const uint8_t * macro)
{
	unsigned char size;

	while ((size = *macro++) != 0)
	{
		nRFCMD_ReadWriteBuffer(macro, NULL, size - 1);
		macro += size;
	}
}

void nRFCMD_RegisterDump(void)
{
	uint8_t t, size, reg, buf[32];

	reg = 0;
	debug_printf("\nnRFCMD_RegisterDump:\n");
	while (((size = nRFCMD_GetRegSize(reg)) > 0) && (reg < 0xFF))
	{
		nRFCMD_RegReadBuf(reg, buf, size);

		debug_printf("\treg[0x%02X]:", reg);
		for (t = 0; t < size; t++)
			debug_printf(" 0x%02X", buf[t]);
		debug_printf("\n");

		reg++;
	}
	debug_printf("\n");
}

void nRFCMD_Status(void)
{
	debug_printf(" * nRF24L01+: rxed=%u\n", g_packet_rxed);
}

void WAKEUP_IRQHandlerPIO1_9(void)
{
	portBASE_TYPE xTaskWoken = pdFALSE;

	xTaskWoken = xSemaphoreGiveFromISR(xnRF_SemaphoreACK, &xTaskWoken);

	/* Clear pending IRQ */
	LPC_SYSCON->STARTRSRP0CLR = STARTxPRP0_PIO1_9;

	portEND_SWITCHING_ISR (xTaskWoken);
}

uint8_t nRFCMD_WaitRx(uint32_t ticks)
{
	if (xSemaphoreTake(xnRF_SemaphoreACK, ticks))
	{
		g_packet_rxed++;
		return pdTRUE;
	}
	else
		return pdFALSE;
}

void nRFCMD_Init(void)
{
	/* setup SPI chipselect pin */
	spi_init_pin(SPI_CS_NRF);

	/* initialize semaphores */
	vSemaphoreCreateBinary(xnRF_SemaphoreACK);

	/* setup IOs */
    /*
	LPC_IOCON->PIO0_7 = 0;
	GPIOSetDir(RF_IRQ_CPU_PORT, RF_IRQ_CPU_PIN, 0);
	NVIC_EnableIRQ(WAKEUP_PIO0_7_IRQn);
	LPC_SYSCON->STARTAPRP0 = (LPC_SYSCON->STARTAPRP0 & ~STARTxPRP0_PIO0_7);
	LPC_SYSCON->STARTRSRP0CLR = STARTxPRP0_PIO0_7;
	LPC_SYSCON->STARTERP0 |= STARTxPRP0_PIO0_7;
*/
	GPIOSetDir(CPU_CE_RF_PORT, CPU_CE_RF_PIN, 1);
	GPIOSetValue(CPU_CE_RF_PORT, CPU_CE_RF_PIN, 0);

	//LPC_IOCON->PIO0_2 = 0;
	//GPIOSetDir(CPU_SWITCH_RF_PORT, CPU_SWITCH_RF_PIN, 1);
	//GPIOSetValue(CPU_SWITCH_RF_PORT, CPU_SWITCH_RF_PIN, 0);
}
