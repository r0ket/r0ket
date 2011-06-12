/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#include <openbeacon.h>
#ifndef UART_DISABLE
#include <ctype.h>
#include <uart.h>
#include <debug_printf.h>
#include <stdarg.h>
#include <string.h>

unsigned long
simple_strtoul (const char *cp, char **endp, unsigned int base)
{
  unsigned long result = 0, value;

  if (*cp == '0')
    {
      cp++;
      if ((*cp == 'x') && isxdigit ((int)cp[1]))
	{
	  base = 16;
	  cp++;
	}
      if (!base)
	{
	  base = 8;
	}
    }
  if (!base)
    {
      base = 10;
    }
  while (isxdigit ((int)*cp) && (value = isdigit ((int)*cp) ? *cp - '0' : (islower ((int)*cp)
								 ?
								 toupper ((int)*cp)
								 : *cp) -
			    'A' + 10) < base)
    {
      result = result * base + value;
      cp++;
    }
  if (endp)
    *endp = (char *) cp;
  return result;
}

long
simple_strtol (const char *cp, char **endp, unsigned int base)
{
  if (*cp == '-')
    return -simple_strtoul (cp + 1, endp, base);
  return simple_strtoul (cp, endp, base);
}

/* we use this so that we can do without the ctype library */
#define is_digit(c)	((c) >= '0' && (c) <= '9')

static int
skip_atoi (const char **s)
{
  int i = 0;

  while (is_digit (**s))
    i = i * 10 + *((*s)++) - '0';
  return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

#define do_div(n,base) ({ \
	int __res; \
	__res = ((unsigned long) n) % base; \
	n = ((unsigned long) n) / base; \
	__res; \
})

static void
number (long num, unsigned int base, int size, int precision,
	int type)
{
  char c, sign;
  static char tmp[66];

  const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
  int i;

  if (type & LARGE)
    digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (type & LEFT)
    type &= ~ZEROPAD;
  if (base < 2 || base > 36)
    return;
  c = (type & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (type & SIGN)
    {
      if (num < 0)
	{
	  sign = '-';
	  num = -num;
	  size--;
	}
      else if (type & PLUS)
	{
	  sign = '+';
	  size--;
	}
      else if (type & SPACE)
	{
	  sign = ' ';
	  size--;
	}
    }
  i = 0;
  if (num == 0)
    tmp[i++] = '0';
  else
    while (num != 0)
      tmp[i++] = digits[do_div (num, base)];
  if (i > precision)
    precision = i;
  size -= precision;
  if (!(type & (ZEROPAD + LEFT)))
    while (size-- > 0)
      default_putchar(' ');
  if (sign)
    default_putchar(sign);
  if (!(type & LEFT))
    while (size-- > 0)
      default_putchar(c);
  while (i < precision--)
    default_putchar('0');
  while (i-- > 0)
    default_putchar(tmp[i]);
  while (size-- > 0)
    default_putchar(' ');
}

static void
tiny_vsprintf (const char *fmt, va_list args)
{
  int len;
  unsigned long num;
  int i, base;
  const char *s;

  int flags;			/* flags to number() */

  int field_width;		/* width of output field */
  int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
  int qualifier;		/* 'h', 'l', or 'q' for integer fields */

  for (; *fmt; ++fmt)
    {
      if (*fmt != '%')
	{
	  if(*fmt=='\n')
	    default_putchar('\r');
	  default_putchar(*fmt);
	  continue;
	}

      /* process flags */
      flags = 0;
    repeat:
      ++fmt;			/* this also skips first '%' */
      switch (*fmt)
	{
	case '-':
	  flags |= LEFT;
	  goto repeat;
	case '+':
	  flags |= PLUS;
	  goto repeat;
	case ' ':
	  flags |= SPACE;
	  goto repeat;
	case '0':
	  flags |= ZEROPAD;
	  goto repeat;
	}

      /* get field width */
      field_width = -1;
      if (is_digit (*fmt))
	field_width = skip_atoi (&fmt);
      else if (*fmt == '*')
	{
	  ++fmt;
	  /* it's the next argument */
	  field_width = va_arg (args, int);
	  if (field_width < 0)
	    {
	      field_width = -field_width;
	      flags |= LEFT;
	    }
	}

      /* get the precision */
      precision = -1;
      if (*fmt == '.')
	{
	  ++fmt;
	  if (is_digit (*fmt))
	    precision = skip_atoi (&fmt);
	  else if (*fmt == '*')
	    {
	      ++fmt;
	      /* it's the next argument */
	      precision = va_arg (args, int);
	    }
	  if (precision < 0)
	    precision = 0;
	}

      /* get the conversion qualifier */
      qualifier = -1;
      if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
	  *fmt == 'Z' || *fmt == 'z' || *fmt == 't' || *fmt == 'q')
	{
	  qualifier = *fmt;
	  if (qualifier == 'l' && *(fmt + 1) == 'l')
	    {
	      qualifier = 'q';
	      ++fmt;
	    }
	  ++fmt;
	}

      /* default base */
      base = 10;

      switch (*fmt)
	{
	case 'c':
	  if (!(flags & LEFT))
	    while (--field_width > 0)
	      default_putchar(' ');
	  default_putchar((unsigned char) va_arg (args, int));
	  while (--field_width > 0)
	    default_putchar(' ');
	  continue;

	case 's':
	  s = va_arg (args, char *);
	  if (!s)
	    s = "<NULL>";

	  len = strnlen (s, precision);

	  if (!(flags & LEFT))
	    while (len < field_width--)
	      default_putchar(' ');
	  for (i = 0; i < len; ++i)
	    default_putchar(*s++);
	  while (len < field_width--)
	    default_putchar(' ');
	  continue;

	case '%':
	  default_putchar('%');
	  continue;

	  /* integer number formats - set up the flags and "break" */
	case 'o':
	  base = 8;
	  break;

	case 'X':
	  flags |= LARGE;
	case 'x':
	  base = 16;
	  break;

	case 'd':
	case 'i':
	  flags |= SIGN;
	case 'u':
	  break;

	default:
	  default_putchar('%');
	  if (*fmt)
	    default_putchar(*fmt);
	  else
	    --fmt;
	  continue;
	}

      if (qualifier == 'l')
	{
	  num = va_arg (args, unsigned long);
	}
      else if (qualifier == 'Z' || qualifier == 'z')
	{
	  num = va_arg (args, size_t);
	}
      else if (qualifier == 'h')
	{
	  num = (unsigned short) va_arg (args, int);
	  if (flags & SIGN)
	    num = (short) num;
	}
      else if (flags & SIGN)
	num = va_arg (args, int);
      else
	num = va_arg (args, unsigned int);
      number (num, base, field_width, precision, flags);
    }
}

void
debug_printf (const char *fmt, ...)
{
  va_list args;

  va_start (args, fmt);
  tiny_vsprintf (fmt, args);
  va_end (args);
}

void hex_dump (const unsigned char *buf, unsigned int addr, unsigned int len)
{
        unsigned int start, i, j;
        char c;

        start = addr & ~0xf;

        for (j=0; j<len; j+=16) {
                debug_printf("%08x:", start+j);

                for (i=0; i<16; i++) {
                        if (start+i+j >= addr && start+i+j < addr+len)
                                debug_printf(" %02x", buf[start+i+j]);
                        else
                                debug_printf("   ");
                }
                debug_printf("  |");
                for (i=0; i<16; i++) {
                        if (start+i+j >= addr && start+i+j < addr+len) {
                                c = buf[start+i+j];
                                if (c >= ' ' && c < 127)
                                        debug_printf("%c", c);
                                else
                                        debug_printf(".");
                        } else
                                debug_printf(" ");
                }
                debug_printf("|\n\r");
        }
}
#endif /* UART_DISABLE */
