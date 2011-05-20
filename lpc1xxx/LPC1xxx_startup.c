/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2010, Roel Verdult
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

// These are defined and created by the linker, locating them in memory
extern unsigned char _etext;
extern unsigned char _data;
extern unsigned char _edata;
extern unsigned char _bss;
extern unsigned char _ebss;

// Prototype the required startup functions
extern int main(void);

// The entry point of the application, prepare segments,
// initialize the cpu and execute main()
void boot_entry(void)
{
  register unsigned char *src, *dst;

  // Get physical data address and copy it to sram
  src = &_etext;
  dst = &_data;
  while(dst < &_edata) {
    *dst++ = *src++;
  }

  // Clear the bss segment
  dst = &_bss;
  while(dst < &_ebss) {
    *dst++ = 0;
  }

  // Execute the code at the program entry point
  main();

  // Do nothing when returned from main, just keep looping
  while(1);
}
