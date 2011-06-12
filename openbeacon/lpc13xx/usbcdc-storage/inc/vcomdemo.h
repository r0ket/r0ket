/*----------------------------------------------------------------------------
 *      Name:    vcomdemo.h
 *      Purpose: USB virtual COM port Demo Definitions
 *      Version: V1.02
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC microcontroller devices only. Nothing else 
 *      gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

/* Push Button Definitions */
#define S2     0x00000400	/* P2.10 */

/* LED Definitions */
#define LED1   0x00000001	/* P2.00 */
#define LED2   0x00000002	/* P2.01 */
#define LED3   0x00000004	/* P2.02 */
#define LED4   0x00000008	/* P2.03 */
#define LED5   0x00000010	/* P2.04 */
#define LED6   0x00000020	/* P2.05 */
#define LED7   0x00000040	/* P2.06 */
#define LED8   0x00000080	/* P2.07 */

#define LEDMSK 0x000000FF	/* P2.0..7 */
