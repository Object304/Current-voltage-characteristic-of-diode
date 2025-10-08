#include "MCP4921.h"

#define MCP4921_CS_LOW() (GPIOA->BSRR = GPIO_BSRR_BR_4)
#define MCP4921_CS_HIGH() (GPIOA->BSRR = GPIO_BSRR_BS_4)

void SPI1_Init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	GPIOA->MODER |= GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER7_1;
	GPIOA->AFR[0] |= (5 << (5*4)) | (5 << (7*4));

	MCP4921_CS_HIGH();

	SPI1->CR1 = 0;
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSM;
	SPI1->CR1 |= SPI_CR1_SSI;
	SPI1->CR2 = 0;
	SPI1->CR2 |= (0xF << SPI_CR2_DS_Pos);
	SPI1->CR1 |= SPI_CR1_SPE;
}

void MCP4921_Send(uint16_t value12) {
	uint16_t data;

	// BUF = 1, GA = 1 (1x), SHDN = 1 (on)
	data = (7 << 12) | (value12 & 0x0FFF);
	MCP4921_CS_LOW();
	while (!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = data;
	while (SPI1->SR & SPI_SR_BSY);
	MCP4921_CS_HIGH();
}
