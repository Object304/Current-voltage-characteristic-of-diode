#ifndef INC_BUFFER_H_
#define INC_BUFFER_H_

#include "stm32f303xe.h"

#define BUF_SIZE 10

typedef struct {
	float data[BUF_SIZE];
	uint16_t size;
} Buffer;

void init_buffer(Buffer* buf);
int add_to_end(float num, Buffer* buf);
int get_from_end(float* num, Buffer* buf);
float get_avg(Buffer* buf);

#endif /* INC_BUFFER_H_ */
