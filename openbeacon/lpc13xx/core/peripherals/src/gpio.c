/*****************************************************************************
 *   gpio.c:  GPIO C file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.20  ver 1.00    Preliminary version, first Release
 *   2009.12.09  ver 1.05    Mod to use mask registers for GPIO writes + inlining (.h)
 *
*****************************************************************************/
#include "LPC13xx.h"		/* LPC13xx Peripheral Registers */
#include "gpio.h"

LPC_GPIO_TypeDef (*const LPC_GPIO[4]) = {
    LPC_GPIO0,
    LPC_GPIO1,
    LPC_GPIO2,
    LPC_GPIO3
};

/*****************************************************************************
** Function name:		GPIOInit
**
** Descriptions:		Initialize GPIO, install the
**						GPIO interrupt handler
**
** parameters:			None
** Returned value:		true or false, return false if the VIC table
**						is full and GPIO interrupt handler can be
**						installed.
** 
*****************************************************************************/
void GPIOInit(void)
{
    /* Enable AHB clock to the GPIO domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
