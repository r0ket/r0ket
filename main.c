#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/smallfonts.h"
#include "lcd/ubuntu18.h"

#include "pmu/pmu.h"
#include "eeprom/eeprom.h"

void ReinvokeISP(void);

/**************************************************************************/

int main(void) {
    // Configure cpu and mandatory peripherals
    systemInit();

    //enable clocks to adc and watchdog
    pmuInit();

    // initialise basic badge functions
    rbInit();
  
    lcdInit(); // display
  
    //Make PIO1_11 an analog input
    gpioSetDir(RB_LED3, gpioDirection_Input);
    IOCON_PIO1_11 = 0x41;

    adcInit();

    lcdFill(255);
    lcdDisplay(0);
    uint32_t j=0;

    //disable the JTAG on PIO3_3
    IOCON_PIO3_3 = 0x10;

    int yctr=8;
    int dx=0;

    font_direction = FONT_DIR_LTR; // LeftToRight is the default
    font = &Font_8x8;

    static FONT fonts[] = {
	 &Font_7x8, 
	 &Font_Ubuntu18pt, // 3 byte-font
	 &Font_8x8,
    };

    int fontctr=0;
    yctr=18;

    uint8_t written = 0;
    uint8_t eeprom_val = 0;

    while (1) {
	lcdDisplay(j);
	delayms(10);

	font=fonts[fontctr];

	if (!written) {
	    if (eeprom_ready()) {
		if (eeprom_write_byte(127,15,42)) {
		    DoString(1, yctr, "Write OK!");
		    written++;
		} else {
		    DoString(1, yctr, "Write NOK!");
		}
	    } else {
		DoString(1, yctr, "NOT READY!");
	    }
	} else {
	    if (eeprom_ready()) {
		if (eeprom_read_byte(127,15,&eeprom_val)) {
		    if (eeprom_val == 42) {
			DoString(1, yctr, "verified!");
		    } else {
			DoString(1, yctr, "failed!");
		    }
		} else {
		    DoString(1, yctr, "Read NOK!");
		}
	    } else {
		DoString(1, yctr, "NOT READY!!");
	    }
	}

	if(1 && gpioGetValue(RB_LED3) == 0){
		gpioSetValue (RB_LED3, 0); 
		while(gpioGetValue(RB_LED3) == 0){
		};
		gpioSetValue (RB_LED3, 1); 
		lcdFill(255);
		fontctr++;
		if(fontctr > 2) {
		    fontctr = 0;
		}
	};

	uint32_t results = adcRead(7);

	results = adcRead(1);
	dx=DoString(0,yctr+20,"Voltage:");
	results *= 10560;
	results /= 1024;
	DoInt(dx,yctr+20,results);

	if( results < 3500 ){
	    DoString(0,yctr+30,"Shutdown");
	    gpioSetValue (RB_PWR_GOOD, 0); 
	    gpioSetValue (RB_LCD_BL, 0); 
	    SCB_SCR |= SCB_SCR_SLEEPDEEP;
	    PMU_PMUCTRL = PMU_PMUCTRL_DPDEN_DEEPPOWERDOWN;
	    __asm volatile ("WFI");
	}else{
	    DoString(0,yctr+30,"OK           ");
	}

    }

    return 0;

}

/* This data must be global so it is not read from the stack */
typedef void (*IAP)(uint32_t [], uint32_t []);
IAP my_iap_entry = (IAP)0x1fff1ff1;
uint32_t command[5], result[4];

/* This function resets some microcontroller peripherals to reset
   hardware configuration to ensure that the USB In-System Programming module
   will work properly. It is normally called from reset and assumes some reset
   configuration settings for the MCU.
   Some of the peripheral configurations may be redundant in your specific
   project.
   */

void ReinvokeISP(void) {
	command[0] = 57;
	/* Disable SYSTICK timer and interrupt before calling into ISP */
	SYSTICK_STCTRL &= ~(SYSTICK_STCTRL_ENABLE | SYSTICK_STCTRL_TICKINT);
	/* make sure USB clock is turned on before calling ISP */
	SCB_SYSAHBCLKCTRL |= 0x04000;
	/* make sure 32-bit Timer 1 is turned on before calling ISP */
	SCB_SYSAHBCLKCTRL |= 0x00400;
	/* make sure GPIO clock is turned on before calling ISP */
	SCB_SYSAHBCLKCTRL |= 0x00040;
	/* make sure IO configuration clock is turned on before calling ISP */
	SCB_SYSAHBCLKCTRL |= 0x10000;
	/* make sure AHB clock divider is 1:1 */
	SCB_SYSAHBCLKDIV = 1;
	/* Send Reinvoke ISP command to ISP entry point*/
        gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, CFG_LED_ON); 

	/* Set stack pointer to ROM value (reset default) This must be the last
	   piece of code executed before calling ISP, because most C expressions
	   and function returns will fail after the stack pointer is changed. */

	//__set_MSP(*((uint32_t *)0x1FFF0000)); /* inline asm function */

	*((uint32_t *)(0x10000054)) = 0x0;

	//set stack pointer
	__asm("  ldr    r0, =%0\n"
			"  mov    sp, r0\n"
			:
			:  "i" (0x1FFF0000));
		


	/* Enter ISP. We call "iap_entry" to enter ISP because the ISP entry is done
	   through the same command interface as IAP. */
	__asm(
			"  ldr   r0, =0x39\n"
			"  ldr   r1, =0x39\n"
			"  ldr   r2, =0x39\n"
			"  ldr   r3, =0x39\n"
			" push {r3}\n"
			"  ldr   r4,=0x1fff1ff1\n"
			"  bx   r4\n"
			);
	//my_iap_entry(command, result);
	// Not supposed to come back!
}
