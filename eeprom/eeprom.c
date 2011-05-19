#include "i2c/i2c.h"
#include "eeprom.h"

uint8_t eeprom_ready() {
    uint32_t state;

    I2CMasterBuffer[0] = EEPROM_BASE_ADDR; // Slave address, page 0, write
    I2CWriteLength = 1; // just write the slave address and stop
    I2CReadLength = 0;

    state = i2cEngine();

    if (state == I2CSTATE_ACK) {
        return 1; // ready for next operation
    } else {
        return 0;
    }
}

uint8_t eeprom_write_byte(uint8_t page_addr, uint8_t byte_addr, uint8_t value) {
    uint8_t i2c_addr;
    uint8_t intern_byte_addr;
    uint32_t state;

    // calculate our addresses with evil bitshift magic
    i2c_addr = EEPROM_BASE_ADDR | (page_addr >> 3) & 0xFE; // bits 7-4 = base address, 3-1 = bits 6-4 of page address, 0 = R/W
    intern_byte_addr = (page_addr << 4) | (byte_addr & 0x0F); // bits 7-4 = bits 3-0 of page address, 3-0 = lower 4 bits of byte address

    I2CMasterBuffer[0] = i2c_addr;
    I2CMasterBuffer[1] = intern_byte_addr;
    I2CMasterBuffer[2] = value;
    I2CWriteLength = 3;
    I2CReadLength = 0;

    state = i2cEngine();

    if (state == I2CSTATE_ACK) {
        return 1; // ready for next operation
    } else {
        return 0;
    }
}

uint8_t eeprom_read_byte(uint8_t page_addr, uint8_t byte_addr, uint8_t *value) {
    uint8_t i2c_addr;
    uint8_t intern_byte_addr;
    uint32_t state;

    // calculate our addresses with evil bitshift magic
    i2c_addr = EEPROM_BASE_ADDR | (page_addr >> 3) & 0xFE; // bits 7-4 = base address, 3-1 = bits 6-4 of page address, 0 = R/W
    intern_byte_addr = (page_addr << 4) | (byte_addr & 0x0F); // bits 7-4 = bits 3-0 of page address, 3-0 = lower 4 bits of byte address

    I2CMasterBuffer[0] = i2c_addr;
    I2CMasterBuffer[1] = intern_byte_addr;
    I2CMasterBuffer[2] = i2c_addr | READ_WRITE;
    I2CWriteLength = 2;
    I2CReadLength = 1;

    state = i2cEngine();

    if (state == I2CSTATE_ACK) {
        *value = I2CSlaveBuffer[0];
        return 1; // ready for next operation
    } else {
        return 0;
    }
}

