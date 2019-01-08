#include "cs43l22.h"
#include "stm32f4xx_hal.h"

#define I2C_ADDR 0x94

#define POWER_CONTROL_1     0x02
#define POWER_CONTROL_2     0x04
#define CLOCKING_CONTROL    0x05
#define INTERFACE_CONTROL_1 0x06
#define MISCELLANEOUS_CONTROLS		0x0E
#define PLAYBACK_CONTROL				0x0F
#define PCM_VOLUME_A						0x1A
#define PCM_VOLUME_B						0x1B

static I2C_HandleTypeDef i2c_device;

static void WriteByte(uint8_t reg, uint8_t byte) {
  uint8_t data[2];
  data[0] = reg;
  data[1] = byte;
	HAL_I2C_Master_Transmit(&i2c_device, I2C_ADDR, data, 2, 100);
}

static uint8_t ReadByte(uint8_t reg) {
  uint8_t read_byte;
	HAL_I2C_Master_Transmit(&i2c_device, I2C_ADDR, &reg, 1, 100);
	HAL_I2C_Master_Receive(&i2c_device, I2C_ADDR, &read_byte, 1, 100);
  return read_byte;
}

void CS43L22Init(I2C_HandleTypeDef i2c_dev) {
  i2c_device = i2c_dev;

  // Set !reset to high to enable the chip.
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);

  // Disable power to the chip.  This is to prevent pops while changing settings.
  WriteByte(POWER_CONTROL_1, 0x01);

  // Enable the headphone output (1010xxxx) and disable the speaker output
  // (xxxx1111).
  WriteByte(POWER_CONTROL_2, 0xaf);

  // The CS43L22 can automatically detect the input clock speed when acting as
  // a slave.  Enable that.
  WriteByte(CLOCKING_CONTROL, 0x01);

  // Interface control.
  WriteByte(INTERFACE_CONTROL_1,
      (0 << 7) |  // Slave not master
      (0 << 6) |  // Non-inverted clock
      (0 << 5) |  // Reserved, must be zero
      (0 << 4) |  // No DSP
      (1 << 2) |  // 01 = I2S data format
      (3 << 0));  // 11 = 16 bit data format
}

void CS43L2Start() {
  // Start the chip using the series of commands on page 32 of the datasheet
  // (https://www.mouser.com/ds/2/76/CS43L22_F2-1142121.pdf)
  WriteByte(0x00, 0x99);
  WriteByte(0x47, 0x80);
  WriteByte(0x32, ReadByte(0x32) | (1 << 7));
  WriteByte(0x32, ReadByte(0x32) & ~(1 << 7));
  WriteByte(0x00, 0x00);

  // Finally, enable power to the chip again.
  WriteByte(POWER_CONTROL_1, 0x9E);
}

void CS43L2Stop() {
  WriteByte(POWER_CONTROL_1, 0x01);
}
