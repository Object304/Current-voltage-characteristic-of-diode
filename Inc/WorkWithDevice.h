#ifndef INC_WORKWITHDEVICE_H_
#define INC_WORKWITHDEVICE_H_

#include "stm32f303xe.h"
#include "ADS1115.h"
#include "MCP4921.h"
#include "USART.h"
#include "Buffer.h"

extern uint16_t MCP4921_Value;

void Periphery_Init();
void Start_Test();
void Start_Work();
void delay_us(uint32_t us);
void RDY_Check(Buffer* buf);

#endif /* INC_WORKWITHDEVICE_H_ */
