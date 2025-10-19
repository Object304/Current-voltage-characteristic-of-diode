#include "USART.h"

void USART2_Init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
	GPIOA->AFR[0] |= (7 << 8) | (7 << 12);
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	USART2->BRR = SystemCoreClock / 9600;
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_UE;
}

void USART2_SendByte(uint8_t byte) {
	while (!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = byte;
}

void USART2_SendFloats(float x, float y) {
	uint8_t buf[1 + 4 + 4 + 1]; // SYNC + x + y + CRC
	uint8_t crc = 0;
	uint16_t idx = 0;

	buf[idx++] = 0xAA;
	crc = 0xAA;

	for (int i = 0; i < 4; i++) {
		buf[idx] = (*(uint32_t*)&x >> (i * 8)) & 0xFF;
		crc ^= buf[idx];
		idx++;
	}

	for (int i = 0; i < 4; i++) {
		buf[idx] = (*(uint32_t*)&y >> (i * 8)) & 0xFF;
		crc ^= buf[idx];
		idx++;
	}

	buf[idx++] = crc;

	for (uint8_t i = 0; i < idx; i++) {
		USART2_SendByte(buf[i]);
	}
}
