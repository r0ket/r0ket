#include <openbeacon.h>

#define TONES_MAX 32
#define ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

/* 8xLED bar display */
static void
set_led (uint8_t led)
{
  led = ~led;
  LPC_GPIO2->MASKED_ACCESS[0xF0] = led;
  LPC_GPIO3->MASKED_ACCESS[0x0F] = led;
}

static void
beep (double frequency)
{
  uint32_t t;

  LPC_TMR32B1->TCR = 0;
  if (frequency)
    {
      t = (SystemCoreClock / 2) / frequency;

      LPC_TMR32B1->MR0 = LPC_TMR32B1->MR1 = t;

      if (LPC_TMR32B1->TC >= t)
	LPC_TMR32B1->TC = t;

      LPC_TMR32B1->TCR = 1;
    }
}

static double
get_frequency_for_tone (uint8_t tone)
{
  static const double frequency[] = {
    262.63, 293.66, 329.63,
    349.23, 392.00, 440.00, 493.88
  };
  return frequency[tone % ARRAY_COUNT (frequency)] *
    (1 << (tone / ARRAY_COUNT (frequency)));
}

static void
tone (uint8_t tone)
{
  static uint8_t lasttone = 0;

  if (tone != lasttone)
    {
      lasttone = tone;
      beep (tone ? get_frequency_for_tone (tone - 1) : 0);
    }
}

int
main (void)
{
  volatile int t;
  int i, mode, index, start;

  /* Get System Clock */
  SystemCoreClockUpdate ();

  /* Initialize GPIO (sets up clock) */
  GPIOInit ();
  GPIOSetDir (BUT1_PORT, BUT1_PIN, 0);
  GPIOSetDir (BUT2_PORT, BUT2_PIN, 0);

  /* Set LED port pin to output */
  LPC_GPIO2->DIR |= 0xF0;
  LPC_GPIO3->DIR |= 0x0F;

  /* Set sound port to PIO1_1 and PIO1_2 */
  LPC_GPIO1->DIR |= 0x6;
  LPC_IOCON->JTAG_TDO_PIO1_1 = 3;
  LPC_IOCON->JTAG_nTRST_PIO1_2 = 3;

  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);
  LPC_TMR32B1->TCR = 2;
  LPC_TMR32B1->MCR = 1 << 4;
  LPC_TMR32B1->EMR = 1 | (0x3 << 4) | (0x3 << 6);

  mode = 2;
  index = 24;
  while (1)
    {
      /* BUTTON1 press cycles through tones */
      if (!GPIOGetValue (BUT1_PORT, BUT1_PIN))
	{
	  mode = (mode + 1) & 0x7;
	  /* debounce */
	  for (t = 0; t < 1000000; t++);
	}

      /* BUTTON2 plays tone */
      if (!GPIOGetValue (BUT2_PORT, BUT2_PIN))
	{
	  set_led (0xFF);
	  /* debounce */

	  switch (mode)
	    {

	    case 0:
	      index++;
	      if (index > TONES_MAX)
		index = 1;
	      tone (index);
	      for (t = 0; t < 2000000; t++);
	      break;

	    case 1:
	      if (index > 1)
		index--;
	      else
		index = TONES_MAX;
	      tone (index);
	      for (t = 0; t < 2000000; t++);
	      break;

	    case 2:
	      tone (index);
	      break;

	    default:
	      start = get_frequency_for_tone (index) - 500;
	      for (i = 100; i > 0; i--)
		{
		  beep (start + (i * 10));
		  for (t = 0; t < (mode * 5000); t++);
		}
	      beep (0);
	      break;
	    }
	}
      else if (mode <= 2)
	tone (0);

      set_led (1 << mode);
    }
}
