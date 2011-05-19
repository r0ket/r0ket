#ifndef _EEPROM_H
#define _EEPROM_H 1

#define EEPROM_BASE_ADDR 0xA0

uint8_t eeprom_ready();
uint8_t eeprom_write_byte(uint8_t page_addr, uint8_t byte_addr, uint8_t value);
uint8_t eeprom_read_byte(uint8_t page_addr, uint8_t byte_addr, uint8_t *value);

#endif /* _EEPROM_H */
