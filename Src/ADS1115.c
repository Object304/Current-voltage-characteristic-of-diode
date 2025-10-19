#include "ADS1115.h"

void I2C1_Init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	GPIOB->MODER |= (0b10 << 12) | (0b10 << 14);
	GPIOB->AFR[0] |= (4 << (6*4)) | (4 << (7*4));
	GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7; // open-drain
	GPIOB->OSPEEDR |= (0b11 << (6*2)) | (0b11 << (7*2)); // high speed
	GPIOB->PUPDR |= (0b01 << (6*2)) | (0b01 << (7*2)); // pull-up
	I2C1->CR1 &= ~I2C_CR1_PE;

	I2C1->TIMINGR = (2 << 16) | // SDADEL = 2
					(4 << 20) | // SCLDEL = 4
					(39 << 8) | // SCLH = 39 (high period)
					(39 << 0) | // SCLL = 39 (low period)
					(0 << 28); // PRESC = 2

	I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Write(uint8_t addr, uint8_t reg, uint16_t data) {
	I2C1->CR2 = (addr << 1) | (3 << 16) | I2C_CR2_START; // 3 bytes: reg + dataH + dataL
	while(!(I2C1->ISR & I2C_ISR_TXIS));
	I2C1->TXDR = reg;
	while(!(I2C1->ISR & I2C_ISR_TXIS));
	I2C1->TXDR = (data >> 8) & 0xFF;
	while(!(I2C1->ISR & I2C_ISR_TXIS));
	I2C1->TXDR = data & 0xFF;
	while(!(I2C1->ISR & I2C_ISR_TC));
	I2C1->CR2 |= I2C_CR2_STOP;
}

uint16_t I2C1_Read(uint8_t addr, uint8_t reg) {
	uint16_t value;

	I2C1->CR2 = (addr << 1) | (1 << 16) | I2C_CR2_START; // 1 byte
	while(!(I2C1->ISR & I2C_ISR_TXIS));
	I2C1->TXDR = reg;
	while(!(I2C1->ISR & I2C_ISR_TC));

	I2C1->CR2 = (addr << 1) | I2C_CR2_RD_WRN | (2 << 16) | I2C_CR2_START; // 2 bytes
	while(!(I2C1->ISR & I2C_ISR_RXNE));
	value = (I2C1->RXDR << 8);
	while(!(I2C1->ISR & I2C_ISR_RXNE));
	value |= I2C1->RXDR;

	while(!(I2C1->ISR & I2C_ISR_TC));
	I2C1->CR2 |= I2C_CR2_STOP;

	return value;
}

void ADS1115_Config(uint8_t addr, uint16_t config) {
	I2C1_Write(addr, 0x01, config);
}

float ADS1115_ReadConversion(uint8_t addr, float PGA) {
	return (int16_t)I2C1_Read(addr, 0x00) * PGA / 32768.0f;
}

void ADS1115_SetThresholds(uint8_t addr, uint16_t lo, uint16_t hi) {
	I2C1_Write(addr, 0x02, lo); // Lo_thresh
	I2C1_Write(addr, 0x03, hi); // Hi_thresh
}
