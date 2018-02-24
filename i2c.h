#ifndef __I2C
#define __I2C

#include <cstddef>
#include "stm32f10x_i2c.h"

void i2cInit(void);
uint8_t i2cReadBytes	(uint8_t slave_addr, uint8_t *data, size_t size);
uint8_t i2cWriteBytes	(uint8_t slave_addr, uint8_t *data, size_t size);


#endif
