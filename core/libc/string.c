/*
 * Software License Agreement (BSD License)
 *
 * Based on original stdio.c released by Atmel
 * Copyright (c) 2008, Atmel Corporation
 * All rights reserved.
 *
 * Modified by Roel Verdult, Copyright (c) 2010
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

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <string.h>

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Copies data from a source buffer into a destination buffer. The two buffers
/// must NOT overlap. Returns the destination buffer.
/// \param pDestination  Destination buffer.
/// \param pSource  Source buffer.
/// \param num  Number of bytes to copy.
//------------------------------------------------------------------------------
void * memcpy(void *pDestination, const void *pSource, size_t num)
{
    unsigned char *pByteDestination;
    unsigned char *pByteSource;
    unsigned int *pAlignedSource = (unsigned int *) pSource;
    unsigned int *pAlignedDestination = (unsigned int *) pDestination;

    // If num is more than 4 bytes, and both dest. and source are aligned,
    // then copy dwords
    if ((((unsigned int) pAlignedDestination & 0x3) == 0)
        && (((unsigned int) pAlignedSource & 0x3) == 0)
        && (num >= 4)) {

        while (num >= 4) {

            *pAlignedDestination++ = *pAlignedSource++;
            num -= 4;
        }
    }

    // Copy remaining bytes
    pByteDestination = (unsigned char *) pAlignedDestination;
    pByteSource = (unsigned char *) pAlignedSource;
    while (num--) {

        *pByteDestination++ = *pByteSource++;
    }

    return pDestination;
}

//------------------------------------------------------------------------------
/// Fills a memory region with the given value. Returns a pointer to the
/// memory region.
/// \param pBuffer  Pointer to the start of the memory region to fill
/// \param value    Value to fill the region with
/// \param num      Size to fill in bytes
//------------------------------------------------------------------------------
void * memset(void *pBuffer, int value, size_t num)
{
    unsigned char *pByteDestination;
    unsigned int  *pAlignedDestination = (unsigned int *) pBuffer;
    unsigned int  alignedValue = (value << 24) | (value << 16) | (value << 8) | value;

    // Set words if possible
    if ((((unsigned int) pAlignedDestination & 0x3) == 0) && (num >= 4)) {
        while (num >= 4) {
            *pAlignedDestination++ = alignedValue;
            num -= 4;
        }
    }
    // Set remaining bytes
    pByteDestination = (unsigned char *) pAlignedDestination;
    while (num--) {
        *pByteDestination++ = value;
    }
    return pBuffer;
}

void* memmove(void *s1, const void *s2, size_t n)
{
  char *s=(char*)s2, *d=(char*)s1;
  
  if(d > s){
    s+=n-1;
    d+=n-1;
    while(n){
      *d--=*s--;
      n--;
    }
  }else if(d < s)
    while(n){
      *d++=*s++;
      n--;
    }
  return s1;
}

int memcmp(const void *av, const void *bv, size_t len)
{
  const unsigned char *a = av;
  const unsigned char *b = bv;
  size_t i;

  for (i=0; i<len; i++) 
  {
    if (a[i] != b[i]) 
    {
      return (int)(a[i] - b[i]);
    }
  }
  return 0;
}



//-----------------------------------------------------------------------------
/// Search a character in the given string.
/// Returns a pointer to the character location.
/// \param pString   Pointer to the start of the string to search.
/// \param character The character to find.
//-----------------------------------------------------------------------------
char * strchr(const char *pString, int character)
{
    char * p = (char *)pString;
    char   c = character & 0xFF;

    while(*p != c) {
        if (*p == 0) {
            return 0;
        }
        p++;
    }
    return p;
}

//-----------------------------------------------------------------------------
/// Return the length of a given string
/// \param pString Pointer to the start of the string.
//-----------------------------------------------------------------------------
size_t strlen(const char *pString)
{
    unsigned int length = 0;

    while(*pString++ != 0) {
        length++;
    }
    return length;
}


//-----------------------------------------------------------------------------
/// Search a character backword from the end of given string.
/// Returns a pointer to the character location.
/// \param pString   Pointer to the start of the string to search.
/// \param character The character to find.
//-----------------------------------------------------------------------------
char * strrchr(const char *pString, int character)
{
    char *p = 0;

    while(*pString != 0) {
        if (*pString++ == character) {
            p = (char*)pString;
        }
    }
    return p;
}

//-----------------------------------------------------------------------------
/// Copy from source string to destination string
/// Return a pointer to the destination string
/// \param pDestination Pointer to the destination string.
/// \param pSource      Pointer to the source string.
//-----------------------------------------------------------------------------
char * strcpy(char *pDestination, const char *pSource)
{
    char *pSaveDest = pDestination;

    for(; (*pDestination = *pSource) != 0; ++pSource, ++pDestination);
    return pSaveDest;
}

//-----------------------------------------------------------------------------
/// Compare the first specified bytes of 2 given strings
/// Return 0 if equals
/// Return >0 if 1st string > 2nd string
/// Return <0 if 1st string < 2nd string
/// \param pString1 Pointer to the start of the 1st string.
/// \param pString2 Pointer to the start of the 2nd string.
/// \param count    Number of bytes that should be compared.
//-----------------------------------------------------------------------------
int strncmp(const char *pString1, const char *pString2, size_t count)
{
    int r;

    while(count) {
        r = *pString1 - *pString2;
        if (r == 0) {
            if (*pString1 == 0) {
                break;
            }
            pString1++;
            pString2++;
            count--;
            continue;
        }
        return r;
    }
    return 0;
}

//-----------------------------------------------------------------------------
/// Copy the first number of bytes from source string to destination string
/// Return the pointer to the destination string.
/// \param pDestination Pointer to the start of destination string.
/// \param pSource      Pointer to the start of the source string.
/// \param count        Number of bytes that should be copied.
//-----------------------------------------------------------------------------
char * strncpy(char *pDestination, const char *pSource, size_t count)
{
    char *pSaveDest = pDestination;

    while (count) {
        *pDestination = *pSource;
        if (*pSource == 0) {
            break;
        }
        pDestination++;
        pSource++;
        count--;
    }
    return pSaveDest;
}

// Following code is based on the BSD licensed code released by UoC 
// Copyright (c) 1988 Regents of the University of California

int strcmp(const char *s1, const char *s2)
{
  while (*s1 == *s2++)
    if (*s1++ == 0)
      return (0);
  return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

char *strtok_r(char *s, const char *delim, char **last);

char *strtok(char *s, const char *delim)
{
  static char *last;
  return strtok_r(s, delim, &last);
}

char *strtok_r(char *s, const char *delim, char **last)
{
  char *spanp;
  int c, sc;
  char *tok;
  
  
  if (s == NULL && (s = *last) == NULL)
    return (NULL);
  
  /*
   * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
   */
cont:
  c = *s++;
  for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
    if (c == sc)
      goto cont;
  }
  
  if (c == 0) {           /* no non-delimiter characters */
    *last = NULL;
    return (NULL);
  }
  tok = s - 1;
  
  /*
   * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
   * Note that delim must have one NUL; we stop if we see that, too.
   */
  for (;;) {
    c = *s++;
    spanp = (char *)delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0)
          s = NULL;
        else
          s[-1] = 0;
        *last = s;
        return (tok);
      }
    } while (sc != 0);
  }
  /* NOTREACHED */
}
