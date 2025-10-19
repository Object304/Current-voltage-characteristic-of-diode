#include "WorkWithDevice.h"

#define ADS1115_ADDR 0b1001000 // if ADDR on GND

float Voltage;
float Current;
uint8_t Voltage_Measuring = 1;
uint16_t MCP4921_Value = 1;

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

void RDY_Check(Buffer* buf) {
	if (GPIOA->IDR & GPIO_IDR_0) {
		float result = ADS1115_ReadConversion(ADS1115_ADDR, 2.048);
		delay_us(10);
		add_to_end(result, buf);
	}
	if (buf->size == 10) {
		float avg = get_avg(buf);
		init_buffer(buf);
		if (Voltage_Measuring == 1) {
			Voltage = avg;
			Voltage_Measuring = 0;

			ADS1115_SetThresholds(ADS1115_ADDR, 0x0000, 0x8000);
			uint16_t config =	(0x01 << 15) | // OS = 1 (start conversion)
								(0x05 << 12) | // MUX = AIN1-GND
								(0x02 << 9) | // PGA = 2.048V
								(0x00 << 8) | // MODE = continuous
								(0x04 << 5) | // DR = 128 SPS
								(0x00 << 3) | // COMP_POL = Active low (default)
								(0x00 << 2) | // COMP_LAT = Non-latching
								(0x00 << 0); // COMP_QUE = Assert after one conversion
			ADS1115_Config(ADS1115_ADDR, config);
		}
		else {
			Current = avg / 55.0f;
			Voltage_Measuring = 1;

			USART2_SendFloats(Voltage, Current);

			MCP4921_Value += 10;
			MCP4921_Send(MCP4921_Value);
			delay_us(150);

			ADS1115_SetThresholds(ADS1115_ADDR, 0x0000, 0x8000);
			uint16_t config =	(0x01 << 15) | // OS = 1 (start conversion)
								(0x00 << 12) | // MUX = AIN0-AIN1
								(0x02 << 9) | // PGA = 2.048V
								(0x00 << 8) | // MODE = continuous
								(0x04 << 5) | // DR = 128 SPS
								(0x00 << 3) | // COMP_POL = Active low (default)
								(0x00 << 2) | // COMP_LAT = Non-latching
								(0x00 << 0); // COMP_QUE = Assert after one conversion
			ADS1115_Config(ADS1115_ADDR, config);
		}

	}
}

void Start_Work() {
	MCP4921_Send(MCP4921_Value);
	delay_us(150); // OPAMP setting delay
	ADS1115_SetThresholds(ADS1115_ADDR, 0x0000, 0x8000);
	uint16_t config =	(0x01 << 15) | // OS = 1 (start conversion)
						(0x00 << 12) | // MUX = AIN0-AIN1
						(0x02 << 9) | // PGA = 2.048V
						(0x00 << 8) | // MODE = continuous
						(0x04 << 5) | // DR = 128 SPS
						(0x00 << 3) | // COMP_POL = Active low (default)
						(0x00 << 2) | // COMP_LAT = Non-latching
						(0x00 << 0); // COMP_QUE = Assert after one conversion
	ADS1115_Config(ADS1115_ADDR, config);
}

void Start_Test() {
//	MCP4921_Send(2482); // 2V
	MCP4921_Send(4095); // 3.3V
	delay_us(150); // OPAMP setting delay
	ADS1115_SetThresholds(ADS1115_ADDR, 0x0000, 0x8000);
	uint16_t config =	(0x01 << 15) | // OS = 1 (start conversion)
						(0x00 << 12) | // MUX = AIN0-AIN1
//						(0x05 << 12) | // MUX = AIN1-GND
						(0x02 << 9) | // PGA = 2.048V
						(0x01 << 8) | // MODE = Single-shot
						(0x04 << 5) | // DR = 128 SPS
//						(0x00 << 3) | // COMP_POL = Active low (default)
						(0x01 << 3) | // COMP_POL = Active high
						(0x00 << 2) | // COMP_LAT = Non-latching
						(0x00 << 0); // COMP_QUE = Assert after one conversion
	ADS1115_Config(ADS1115_ADDR, config);
	float result = ADS1115_ReadConversion(ADS1115_ADDR, 2.048); // must be 0.7



	ADS1115_SetThresholds(ADS1115_ADDR, 0x0000, 0x8000);
	config =	(0x01 << 15) | // OS = 1 (start conversion)
//						(0x00 << 12) | // MUX = AIN0-AIN1
						(0x05 << 12) | // MUX = AIN1-GND
						(0x02 << 9) | // PGA = 2.048V
						(0x01 << 8) | // MODE = Single-shot
						(0x04 << 5) | // DR = 128 SPS
//						(0x00 << 3) | // COMP_POL = Active low (default)
						(0x01 << 3) | // COMP_POL = Active high
						(0x00 << 2) | // COMP_LAT = Non-latching
						(0x00 << 0); // COMP_QUE = Assert after one conversion
	ADS1115_Config(ADS1115_ADDR, config);
	result = ADS1115_ReadConversion(ADS1115_ADDR, 2.048) / 55.0f; // must be 0.7
}
