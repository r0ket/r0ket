#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include <string.h>


#include "core/cpu/cpu.h"
#include "core/uart/uart.h"

/**************************************************************************/
#define BUF 5

void main_uart(void) {
	uint8_t uartBuffer[BUF] = { 'T', 'e', 's', 't', '\n' };
uint8_t o[2]={0,0};

	gpioSetDir(RB_LED0, gpioDirection_Output);
	gpioSetValue (RB_LED0, 1);
	lcdPrintln("Hi");
	lcdRefresh();
	uartInit(9600);
	lcdPrintln("Inited.");
	lcdRefresh();

	uartSend((uint8_t *)uartBuffer, BUF);
	lcdPrintln("sent.");
	lcdRefresh();

	uartRxBufferWrite('x');

	// Get a reference to the UART control block
//	uart_pcb_t *pcb = uartGetPCB();

	gpioSetValue (RB_LED0, 1-gpioGetValue(RB_LED0));
	// Read any text available in the queue
	while (uartRxBufferDataPending())
	{
		gpioSetValue (RB_LED0, 1-gpioGetValue(RB_LED0));
		// Read the first available character
		uint8_t c = uartRxBufferRead();
		o[0]=c;
		lcdPrint(o);
		lcdRefresh();
		uartSend(&c, 1);
	}
};
