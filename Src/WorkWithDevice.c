#include "WorkWithDevice.h"

void Periphery_Init() {
	I2C1_Init();
	SPI1_Init();
	USART1_Init();
}

void delay_us(uint32_t us)
{
	uint32_t ticks_per_us = SystemCoreClock / 1000000U;
	SysTick->LOAD = ticks_per_us * us;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;
}

void Start_Test() {
	MCP4921_Send(2482); // 2V
	delay_us(150); // OPAMP setting delay
	uint16_t config =	(0x01 << 15) | // OS = 1 (start conversion)
						(0x04 << 12) | // MUX = AIN0-GND
						(0x02 << 9) | // PGA = 2.048V
						(0x00 << 8) | // MODE = continuous
						(0x04 << 5) | // DR = 128 SPS
						(0x03); // comparator disabled
	ADS1115_Config(ADS1115_ADDR, config);
	float val = ADS1115_ReadConversion(ADS1115_ADDR, 2.048);
}
