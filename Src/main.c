#include "main.h"


int main(void)
{
	Periphery_Init();
	while (1)
	{
		if (sync_byte_received) {
			Start_Work(4095);
			sync_byte_received = 0;
		}
	}
	return 0;
}
