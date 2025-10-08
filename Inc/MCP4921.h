#ifndef INC_MCP4921_H_
#define INC_MCP4921_H_

#include "stm32f303xe.h"

void SPI1_Init();
void MCP4921_Send(uint16_t value12);

#endif /* INC_MCP4921_H_ */

/*
 * USAGE EXAMPLE:
 *
 * SPI1_Init();
 * MCP4921_Send(value12);
 *
 * value12 = 0...4095
 * V_out = V_ref * value12 / 4096
 */
