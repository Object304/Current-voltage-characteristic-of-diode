#ifndef INC_USART_H_
#define INC_USART_H_

#include <stdint.h>
#include "stm32f303xe.h"

void USART1_Init();
void USART1_SendByte(uint8_t byte);
void USART1_SendFloats(float x, float y);

#endif /* INC_USART_H_ */
