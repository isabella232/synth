#ifndef __CS43L22_H__
#define __CS43L22_H__

#include <stdint.h>
#include "stm32f4xx_hal.h"

// Initialize the device to receive analog input.
void CS43L22Init(I2C_HandleTypeDef i2c_handle);

// Start and stop the device.
void CS43L2Start();
void CS43L2Stop();

#endif  // __CS43L22_H__
