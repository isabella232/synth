#ifndef __FIXED_H__
#define __FIXED_H__

#include <stdint.h>

typedef struct Fixed1616 {
	uint32_t full;
} Fixed1616;

Fixed1616 Fixed1616FromFloat(float val);
Fixed1616 Fixed1616FromInt(uint16_t val);
float Fixed1616AsFloat(Fixed1616* fixed);
uint16_t Fixed1616AsInt(Fixed1616* fixed);

void Fixed1616AddInt(Fixed1616* fixed, uint16_t amount);
void Fixed1616Add(Fixed1616* fixed, Fixed1616 amount);

void Fixed1616SubInt(Fixed1616* fixed, uint16_t amount);
void Fixed1616Sub(Fixed1616* fixed, Fixed1616 amount);

void Fixed1616MulInt(Fixed1616* fixed, uint16_t amount);
void Fixed1616Mul(Fixed1616* fixed, Fixed1616 amount);

void Fixed1616DivInt(Fixed1616* fixed, uint16_t amount);
void Fixed1616Div(Fixed1616* fixed, Fixed1616 amount);

#endif  // __FIXED_H__
