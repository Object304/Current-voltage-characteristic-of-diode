#include "main.h"


int main(void)
{
	Periphery_Init();
	Start_Work();
//	Start_Test();
	Buffer buf;
	init_buffer(&buf);
	while (1)
	{
		if (MCP4921_Value < 4095) {
			RDY_Check(&buf);
		}
	}
	return 0;
}
