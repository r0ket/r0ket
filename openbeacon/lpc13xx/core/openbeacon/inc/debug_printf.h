/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#ifndef __DEBUG_PRINTF_H__
#define __DEBUG_PRINTF_H__
#ifdef UART_DISABLE
#define debug_printf(...)
#else /*UART_DISABLE*/
extern void debug_printf (const char *fmt, ...);
extern void hex_dump (const unsigned char *buf, unsigned int addr, unsigned int len);
#endif/*UART_DISABLE*/
#endif/*__DEBUG_PRINTF_H__*/
