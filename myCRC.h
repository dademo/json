#ifndef MYCRC_H_INCLUDED
#define MYCRC_H_INCLUDED

#include <stdint.h>	// uint64_t

uint64_t mkMyCRC(void* data, int dataSize);	// double = 8 bytes
//int32_t mkMyCRC(void* data, int dataSize);	// double = 8 bytes
void binShowData(void* data, int dataSize);

#endif
