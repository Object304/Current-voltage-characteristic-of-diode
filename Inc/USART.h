#ifndef INC_USART_H_
#define INC_USART_H_

#include "stm32f303xe.h"

extern volatile uint8_t sync_byte_received;

void USART2_Init();
void USART2_SendByte(uint8_t byte);
void USART2_SendFloats(float x, float y);

#endif /* INC_USART_H_ */
