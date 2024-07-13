#ifndef __IST8310_H__
#define __IST8310_H__

#include "main.h"
#include "i2c.h"

#define IST8310_IIC_ADDRESS 0x0E  //the I2C address of IST8310
#define MAG_SEN 0.3f

uint8_t ist8310_Init();
void ist8310_Read();
#endif 