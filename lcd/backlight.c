/*
 * Based on microbuilders PWM example.
 */
#include "lpc134x.h"
#include "sysdefs.h"

uint32_t brightness = 100;

void backlightInit(void) {
  /* Enable the clock for CT16B1 */
  SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_CT16B1);

  /* Configure PIO1.10 as Timer1_16 MAT1 Output */
  IOCON_PIO1_10 &= ~IOCON_PIO1_10_FUNC_MASK;
  IOCON_PIO1_10 |= IOCON_PIO1_10_FUNC_CT16B1_MAT1;

  /* Set default duty cycle (MR1) */
  TMR_TMR16B1MR1 = (0xFFFF * (100 - brightness)) / 100;

  /* External Match Register Settings for PWM */
  TMR_TMR16B1EMR = TMR_TMR16B1EMR_EMC1_TOGGLE | TMR_TMR16B1EMR_EM1;

  /* enable Timer1 */
  TMR_TMR16B1TCR = TMR_TMR16B1TCR_COUNTERENABLE_ENABLED;

  /* Enable PWM1 */
  TMR_TMR16B1PWMC = TMR_TMR16B1PWMC_PWM1_ENABLED;
}

int backlightSetBrightness(uint32_t percentage) {
  if ((percentage < 1) || (percentage > 100)) {
    /* brightness must be a value between 1 and 100 */
    return -1;
  }

  /* Set Duty Cycle (MR1) */
  TMR_TMR16B1MR1 = (0xFFFF * (100 - (brightness = percentage))) / 100;

  return 0;
}

uint32_t backlightGetBrightness(void) {
    return brightness;
}
