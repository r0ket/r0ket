#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/backlight.h"
#include "lcd/allfonts.h"
void ReinvokeISP(void);

/**************************************************************************/

void module_adc(void) {
    int yctr = 18;
    int dx = 0;
    uint32_t brightness = 0;
    uint8_t c = 0;

    font_direction = FONT_DIR_LTR; // LeftToRight is the default
    font = &Font_7x8;

    DoString(0, 0, "adc");

    backlightInit();
    brightness = backlightGetBrightness();

    while (1) {
        uint32_t results;
        lcdDisplay(0);
        delayms(10);

        if(gpioGetValue(RB_BTN1)==0){
            brightness++;
            if (brightness > 100) brightness = 100;
            backlightSetBrightness(brightness);
        }

        if(gpioGetValue(RB_BTN0)==0){
            brightness--;
            if (brightness > 100) brightness = 0;
            backlightSetBrightness(brightness);
        }

        if (gpioGetValue(RB_BTN3) == 0) {
            while(gpioGetValue(RB_BTN3)==0);
            lcdInvert();
        }

        if (gpioGetValue(RB_BTN4)==0) {
            while(gpioGetValue(RB_BTN4)==0);
            DoString(0,8,"Enter ISP!");
            lcdDisplay(0);
            EnableWatchdog(1000*5);
            ReinvokeISP();

        }

        dx = DoString(0, yctr, "Bright:");
        dx = DoInt(dx, yctr, brightness);
        DoString(dx, yctr, "%   ");

        results = adcRead(1);
        dx=DoString(0,yctr+20,"Voltage:");
        results *= 10560;
        results /= 1024;
        DoInt(dx,yctr+20,results);

        if (results < 3500) {
            DoString(0,yctr+30,"Shutdown");
            gpioSetValue (RB_PWR_GOOD, 0); 
            gpioSetValue (RB_LCD_BL, 0); 
            SCB_SCR |= SCB_SCR_SLEEPDEEP;
            PMU_PMUCTRL = PMU_PMUCTRL_DPDEN_DEEPPOWERDOWN;
            __asm volatile ("WFI");
        } else if (results < 3550 ){
            DoString(0,yctr+30,"Warning      ");
            IOCON_PIO1_11 = 0x0;
            gpioSetDir(RB_LED3, gpioDirection_Output);
            c++;
            if( c == 1 )
                gpioSetValue (RB_LED3, 1);
            else if( c == 2 )
                gpioSetValue (RB_LED3, 0);
            else if( c == 10 )
                c = 0;
        }else {
            DoString(0,yctr+30,"OK           ");
        }
        results = adcRead(7);
        dx=DoString(0,yctr+40,"Sun:");
        DoInt(dx,yctr+40,results);

        if( results > 340 ){
            backlightSetBrightness(0);
        }else{
            backlightSetBrightness(33); 
        }
            
    }

    return;
}

