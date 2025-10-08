#ifndef INC_ADS1115_H_
#define INC_ADS1115_H_

#include "stm32f303xe.h"

void I2C1_Init();
void I2C1_Write(uint8_t addr, uint8_t reg, uint16_t data);
uint16_t I2C1_Read(uint8_t addr, uint8_t reg);
void ADS1115_Config(uint8_t addr, uint16_t config);
float ADS1115_ReadConversion(uint8_t addr, float PGA);

#endif /* INC_ADS1115_H_ */

/*
 * USAGE EXAMPLE:
 *
 * #define ADS1115_ADDR 0b1001000 // if ADDR on GND
 *
 * I2C1_Init();
 *
 * uint16_t config =	(0x01 << 15) | // OS = 1 (start conversion)
 * 						(0x04 << 12) | // MUX = AIN0-GND
 * 						(0x02 << 9) | // PGA = 2.048V
 * 						(0x01 << 8) | // MODE = single-shot
 * 						(0x04 << 5) | // DR = 128 SPS
 * 						(0x03); // comparator disabled
 *
 * ADS1115_Config(ADS1115_ADDR, config);
 *
 * float val = ADS1115_ReadConversion(ADS1115_ADDR, 2.048);
 *
 * ...work with val...
 */
