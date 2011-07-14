#ifndef _FILETRANSFER_H_
#define _FILETRANSFER_H_
#include <stdint.h>

#define MAXSIZE 1024

int filetransfer_send(uint8_t *filename, uint16_t size,
                uint8_t *mac, uint32_t const k[4]);
int filetransfer_receive(uint8_t *mac, uint32_t const k[4]);
#endif
