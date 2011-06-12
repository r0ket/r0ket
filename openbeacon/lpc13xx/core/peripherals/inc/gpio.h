/*****************************************************************************
 *   gpio.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.09.01  ver 1.00    Preliminary version, first Release
 *   2009.12.09  ver 1.05    Mod to use mask registers for GPIO writes + inlining (.h)
 *
******************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

extern void GPIOInit (void);
extern LPC_GPIO_TypeDef (*const LPC_GPIO[4]);

/*****************************************************************************
** Function name:		GPIOSetValue
**
** Descriptions:		Set/clear a bitvalue in a specific bit position
**						in GPIO portX(X is the port number.)
**
** parameters:			port num, bit position, bit value
** Returned value:		None
**
*****************************************************************************/
static __INLINE void
GPIOSetValue (uint32_t portNum, uint32_t bitPosi, uint32_t bitVal)
{
  LPC_GPIO[portNum]->MASKED_ACCESS[(1 << bitPosi)] = (bitVal << bitPosi);
}

/*****************************************************************************
** Function name:		GPIOGetValue
**
** Descriptions:		Get a bitvalue in a specific bit position
**						in GPIO portX(X is the port number.)
**
** parameters:			port num, bit position
** Returned value:		bit value
**
*****************************************************************************/
static __INLINE uint32_t GPIOGetValue( uint32_t portNum, uint32_t bitPosi)
{
    return LPC_GPIO[portNum]->MASKED_ACCESS[(1<<bitPosi)] ? 1:0;
}

/*****************************************************************************
** Function name:		GPIOSetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			port num, bit position, direction (1 out, 0 input)
** Returned value:		None
**
*****************************************************************************/
static __INLINE void
GPIOSetDir (uint32_t portNum, uint32_t bitPosi, uint32_t dir)
{
  if (dir)
    LPC_GPIO[portNum]->DIR |= 1 << bitPosi;
  else
    LPC_GPIO[portNum]->DIR &= ~(1 << bitPosi);
}

#endif /* end __GPIO_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
