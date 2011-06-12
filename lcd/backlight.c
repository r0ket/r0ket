/*
 * Based on microbuilders PWM example.
 */
#include "lpc134x.h"
#include "sysdefs.h"
#include "core/gpio/gpio.h"
#include "basic/basic.h"

uint32_t brightness = 100;

void backlightInit(void) {
  /* Enable the clock for CT16B1 */
  SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_CT16B1);

  /* Configure PIO1.9 as Timer1_16 MAT0 Output */
  IOCON_PIO1_9 &= ~IOCON_PIO1_9_FUNC_MASK;
  IOCON_PIO1_9 |= IOCON_PIO1_9_FUNC_CT16B1_MAT0;

  /* Set default duty cycle (MR1) */
  TMR_TMR16B1MR0 = (0xFFFF * (100 - brightness)) / 100;

  /* External Match Register Settings for PWM */
  TMR_TMR16B1EMR = TMR_TMR16B1EMR_EMC0_TOGGLE | TMR_TMR16B1EMR_EM0;

  /* enable Timer1 */
  TMR_TMR16B1TCR = TMR_TMR16B1TCR_COUNTERENABLE_ENABLED;

  /* Enable PWM0 */
  TMR_TMR16B1PWMC = TMR_TMR16B1PWMC_PWM0_ENABLED;
 
  // Enable Step-UP
  gpioSetDir(RB_PWR_LCDBL, gpioDirection_Output);
  gpioSetValue(RB_PWR_LCDBL, 0);
}

int backlightSetBrightness(uint32_t percentage) {
  if ((percentage < 0) || (percentage > 100)) {
    /* brightness must be a value between 1 and 100 */
    return -1;
  }

  if( percentage == 0 ){
    gpioSetDir(RB_PWR_LCDBL, gpioDirection_Input);
  }else{
    gpioSetDir(RB_PWR_LCDBL, gpioDirection_Output);
    gpioSetValue(RB_PWR_LCDBL, 0);
  }

  /* Set Duty Cycle (MR1) */
  TMR_TMR16B1MR0 = (0xFFFF * (100 - (brightness = percentage))) / 100;

  return 0;
}

uint32_t backlightGetBrightness(void) {
    return brightness;
}
