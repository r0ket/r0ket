#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sysinit.h"

#include "core/gpio/gpio.h"
#include "core/adc/adc.h"
#include "core/systick/systick.h"

#include "lpc134x.h"
#include "sysdefs.h"

#define CFG_LED_PORT                (1)
#define CFG_LED_PIN                 (11)
#define CFG_LED_ON                  (1)
#define CFG_LED_OFF                 (0)

void ReinvokeISP(void);

#include <lcd/render.h>
#include <lcd/smallfonts.h>
#include <lcd/ubuntu18.h>


/**************************************************************************/

int main(void)
{
  // Configure cpu and mandatory peripherals
  systemInit();

  //enable clocks to adc and watchdog
  pmuInit();

  uint32_t currentSecond, lastSecond;
  currentSecond = lastSecond = 0;
  
  //external vcc on
  gpioSetDir(1, 9, 1);
  gpioSetValue (1, 9, 0); 

  //backlight on
  gpioSetDir(1, 10, 1);
  gpioSetValue (1, 10, 1); 
  
  //gpioSetDir(0, 1, 0);
  //gpioSetValue (0, 1, 1); 

  //gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, CFG_LED_ON); 
//  gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, 0); 
  //pmuSleep();
  //pmuPowerDown();
  init(); // display
  
  //Make PIO1_11 an analog input
  gpioSetDir(CFG_LED_PORT, CFG_LED_PIN, 0);
  IOCON_PIO1_11 = 0x41;
  adcInit();
  
  fill(255);
  display(0);
  uint32_t j=0;

  // Set GPIO3.3 to input
  gpioSetDir(3, 3, gpioDirection_Input);
  // Disable the internal pullup/down resistor
//  gpioSetPullup(&IOCON_PIO3_3, gpioPullupMode_Inactive);
  //disable the JTAG on PIO3_3
  IOCON_PIO3_3 = 0x10;

  int yctr=0;
  int dx=0;

  font_direction=FONT_DIR_LTR; // LeftToRight is the default
  font= &Font_8x8      ;
  /*
  DoString(0,yctr,"Hallo Welt");
  yctr+=9;
  font= &Font_8x8Thin  ;
  DoString(0,yctr,"Hallo Welt");
  yctr+=9;
  font= &Font_7x8;
  DoString(0,yctr,"Hallo Welt");
  yctr+=9;
  font= &Font_5x8;
  DoString(0,yctr,"Hallo Welt");
  yctr+=9;
  font= &Font_3x6;
  DoString(0,yctr,"HALLO WELT");
  yctr+=6;
  */

  yctr+=1;
  yctr=8;

  /* Read & display serial number */
#include "core/iap/iap.h"

/*
  IAP_return_t iap_return;
  iap_return = iapReadSerialNumber();

  if (iap_return.ReturnCode == 0) {
	  int x,y;
	  font= &Font_3x6;
	  x=RESX-(8*font->u8Width)-1;y=28;
	  dx=DoIntX(x,y,iap_return.Result[0]); y+=font->u8Height;
	  dx=DoIntX(x,y,iap_return.Result[1]); y+=font->u8Height;
	  dx=DoIntX(x,y,iap_return.Result[2]); y+=font->u8Height;
	  dx=DoIntX(x,y,iap_return.Result[3]); y+=font->u8Height;
	  font= &Font_7x8;
  };
  */

	static FONT fonts[]=
	{
	 & Font_8x8,
	 & Font_Ubuntu18pt, // 3 byte-font
	 & Font_7x8, 
	};
	int fontctr=0;
	yctr=18;

  while (1)
  {
    display(j);
    delayms(10);
    // Toggle LED once per second ... rollover = 136 years :)
	/*
    currentSecond = systickGetSecondsActive();
    if (currentSecond != lastSecond){
		dx=DoString(0,yctr,"UP:");
		DoInt(dx,yctr,currentSecond);
	};
	*/

	font=fonts[fontctr];
	DoString(1,yctr,"Hey Y!");

	if(1 && gpioGetValue(3,3)==0){
		gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, CFG_LED_ON); 
		while(gpioGetValue(3,3)==0);
		gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, CFG_LED_OFF); 
		fill(255);
		fontctr++;
//		yctr++;
		if(fontctr>2)
			fontctr=0;
	};

    uint32_t results = adcRead(7);
    //dx=DoString(0,yctr+10,"LED:");
    //DoInt(dx,yctr+9,results);

    results = adcRead(1);
    dx=DoString(0,yctr+20,"Voltage:");
    results *= 10560;
    results /= 1024;
    DoInt(dx,yctr+20,results);

    if( results < 3500 ){
        DoString(0,yctr+30,"Shutdown");
        //external vcc off
        gpioSetDir(1, 9, 0);
        gpioSetValue (1, 9, 0); 
        //backlight off
        gpioSetValue (1, 10, 0); 
        SCB_SCR |= SCB_SCR_SLEEPDEEP;
        PMU_PMUCTRL = PMU_PMUCTRL_DPDEN_DEEPPOWERDOWN;
        __asm volatile ("WFI");
    }else{
        DoString(0,yctr+30,"OK           ");
    }

	/*
      lastSecond = currentSecond;
	  yctr++;
//      uint32_t results = adcRead(7);
//      if( results < 400 )
//        gpioSetValue (1, 10, 1); 
//      else
//        gpioSetValue (1, 10, 0); 
        
      if (gpioGetValue(CFG_LED_PORT, CFG_LED_PIN) == CFG_LED_OFF)
      {
        gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, CFG_LED_ON); 
        //gpioSetValue (1, 10, 1); 
      }
      else
      {
        gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, CFG_LED_OFF); 
        //gpioSetValue (1, 10, 0); 
      }
    }
	*/

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

void ReinvokeISP(void)
{
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
//	__set_MSP(*((uint32_t *)0x1FFF0000)); /* inline asm function */

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
//	my_iap_entry(command, result);
	// Not supposed to come back!
}

