#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/smallfonts.h"
#include "lcd/ubuntu18.h"

#include "pmu/pmu.h"
#include "eeprom/eeprom.h"

/**************************************************************************/

void module_eeprom(void) {
    //Make PIO1_11 an analog input
    gpioSetDir(RB_LED3, gpioDirection_Input);
    IOCON_PIO1_11 = 0x41;

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

    return;
}
