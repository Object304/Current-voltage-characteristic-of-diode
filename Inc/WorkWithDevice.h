#ifndef INC_WORKWITHDEVICE_H_
#define INC_WORKWITHDEVICE_H_

#include "stm32f303xe.h"
#include "ADS1115.h"
#include "MCP4921.h"
#include "USART.h"
#include "Buffer.h"

void Periphery_Init();
void Start_Work(uint16_t MCP4921_TargetValue);

#endif /* INC_WORKWITHDEVICE_H_ */
