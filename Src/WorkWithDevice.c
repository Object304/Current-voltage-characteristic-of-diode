#include "WorkWithDevice.h"

#define ADS1115_ADDR 0b1001000 // if ADDR on GND

void Periphery_Init() {
	I2C1_Init();
	SPI1_Init();
	USART2_Init();
}

void delay_us(uint32_t us) {
	uint32_t ticks_per_us = SystemCoreClock / 1000000U;
	SysTick->LOAD = ticks_per_us * us;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;
}

void Start_Work(uint16_t MCP4921_TargetValue) {
	Buffer buf;
	init_buffer(&buf);
	float voltage;
	float current;
	uint16_t voltage_config =	(0x01 << 15) | // OS = 1 (start conversion)
								(0x00 << 12) | // MUX = AIN0-AIN1
								(0x02 << 9) | // PGA = 2.048V
								(0x01 << 8) | // MODE = Single-shot
								(0x07 << 5); // DR = 860 SPS

	uint16_t current_config =	(0x01 << 15) | // OS = 1 (start conversion)
								(0x05 << 12) | // MUX = AIN1-GND
								(0x02 << 9) | // PGA = 2.048V
								(0x01 << 8) | // MODE = Single-shot
								(0x07 << 5); // DR = 860 SPS
	for (volatile uint16_t value = 10; value < MCP4921_TargetValue; value += 30) {
		MCP4921_Send(value);
		delay_us(150); // OPAMP setting delay

		for (uint8_t i = 0; i < 10; i++) {
			ADS1115_Config(ADS1115_ADDR, voltage_config);
			delay_us(1200);
			voltage = ADS1115_ReadConversion(ADS1115_ADDR, 2.048);
			add_to_end(voltage, &buf);
		}
		voltage = get_avg(&buf);
		init_buffer(&buf);

		for (uint8_t i = 0; i < 10; i++) {
			ADS1115_Config(ADS1115_ADDR, current_config);
			delay_us(1200);
			current = ADS1115_ReadConversion(ADS1115_ADDR, 2.048) / 55.0f;
			add_to_end(current, &buf);
		}
		current = get_avg(&buf);
		init_buffer(&buf);

		USART2_SendFloats(voltage, current);
	}
}
