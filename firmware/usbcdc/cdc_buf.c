/*******************************************************************
    Copyright (C) 2009 FreakLabs
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.
*******************************************************************/

/**************************************************************************/
/*! 
    @file     cdc_buf.c
    @author   Christopher Wang (Freaklabs)
              Modified by: K. Townsend (microBuilder.eu)
    @date     19 May 2010

    Original code taken from the FreakUSB Open Source USB Device Stack
    http://freaklabs.org/index.php/FreakUSB-Open-Source-USB-Device-Stack.html

    If it works well, you can thank Akiba at Freaklabs.  If it fails
    miserably, you can blame me (since parts of it it were rather
    ungraciously modified). :-)

*/
/**************************************************************************/

#include "cdc_buf.h"

static cdc_buffer_t cdcfifo;

/**************************************************************************/
/*!
  Gets a pointer to the fifo buffer
*/
/**************************************************************************/
cdc_buffer_t *cdcGetBuffer()
{
  return &cdcfifo;
}

/**************************************************************************/
/*!
  Initialises the RX FIFO buffer
*/
/**************************************************************************/
void cdcBufferInit()
{
  cdcfifo.len = 0;
}

/**************************************************************************/
/*!
  Read one byte out of the RX buffer. This function will return the byte
  located at the array index of the read pointer, and then increment the
  read pointer index.  If the read pointer exceeds the maximum buffer
  size, it will roll over to zero.
*/
/**************************************************************************/
uint8_t cdcBufferRead()
{
  uint8_t data;

  data = cdcfifo.buf[cdcfifo.rd_ptr];
  cdcfifo.rd_ptr = (cdcfifo.rd_ptr + 1) % CFG_USBCDC_BUFFERSIZE;
  cdcfifo.len--;
  return data;
}

/**************************************************************************/
/*!
  Reads x bytes from cdc buffer
 */
/**************************************************************************/
uint32_t cdcBufferReadLen(uint8_t* buf, uint32_t len)
{
  uint32_t counter, actual;
  counter = actual = 0;

  while(counter != len)
  {
    // Make sure we don't exceed buffer limits
    if (cdcfifo.len > 0)
    {
      buf[counter] = cdcBufferRead();
      actual++;
      counter++;
    }
    else
    {
      return actual;
    }
  }

  return actual;
}

/**************************************************************************/
/*!
  Write one byte into the RX buffer. This function will write one
  byte into the array index specified by the write pointer and increment
  the write index. If the write index exceeds the max buffer size, then it
  will roll over to zero.
*/
/**************************************************************************/
void cdcBufferWrite(uint8_t data)
{
  cdcfifo.buf[cdcfifo.wr_ptr] = data;
  cdcfifo.wr_ptr = (cdcfifo.wr_ptr + 1) % CFG_USBCDC_BUFFERSIZE;
  cdcfifo.len++;
}

/**************************************************************************/
/*!
    Clear the fifo read and write pointers and set the length to zero.
*/
/**************************************************************************/
void cdcBufferClearFIFO()
{
  cdcfifo.rd_ptr = 0;
  cdcfifo.wr_ptr = 0;
  cdcfifo.len = 0;
}

/**************************************************************************/
/*!
    Check whether there is any data pending on the RX buffer.
*/
/**************************************************************************/
uint8_t cdcBufferDataPending()
{
  if (cdcfifo.len != 0)
  {
    return 1;
  }

  return 0;
}
