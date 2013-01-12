#include "basic.h"

// Calculate the CRC for transmitted and received data using
// the CCITT 16bit algorithm (X^16 + X^12 + X^5 + 1).

uint16_t crc16(uint8_t * buf, int len){
    unsigned int crc=0xffff;

	for(int i=0;i<len;i++){
		crc  = (unsigned char)(crc >> 8) | (crc << 8);
		crc ^= buf[i];
		crc ^= (unsigned char)(crc & 0xff) >> 4;
		crc ^= (crc << 8) << 4;
		crc ^= ((crc & 0xff) << 4) << 1;
	};
	return crc;
}

/* Note:
   It is best not to alter this code. For example, (crc<<8)<<4 does
   not generate the same code as crc<<12. Although the result of the
   computation is the same, the latter generates much more code and
   executes slower.
 */
