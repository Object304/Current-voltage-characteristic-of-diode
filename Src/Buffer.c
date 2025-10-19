#include "Buffer.h"

void init_buffer(Buffer* buf) {
	buf->size = 0;
}

int add_to_end(float num, Buffer* buf) {
	if (buf->size == BUF_SIZE) {
		return 0;
	}
	buf->data[buf->size] = num;
	buf->size++;
	return 1;
}

int get_from_end(float* num, Buffer* buf) {
	if (buf->size == 0) {
		return 0;
	}
	*num = buf->data[buf->size - 1];
	buf->size--;
	return 1;
}

float get_avg(Buffer* buf) {
	float avg = 0;
	for (uint16_t i = 0; i < buf->size; i++) {
		avg += buf->data[i];
	}
	avg /= (float)buf->size;
	return avg;
}
