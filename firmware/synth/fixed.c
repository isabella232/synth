#include "fixed.h"

#include <stdio.h>

Fixed1616 Fixed1616FromFloat(float val) {
	Fixed1616 fixed;
	uint32_t intval = val;
	fixed.full = intval << 16 | (uint32_t) ((val - intval) * UINT16_MAX);
	return fixed;
}

Fixed1616 Fixed1616FromInt(uint16_t val) {
	Fixed1616 fixed;
	fixed.full = val;
	fixed.full <<= 16;
	return fixed;
}

float Fixed1616AsFloat(Fixed1616* fixed) {
	return (fixed->full >> 16) + (float) (fixed->full & 0xFFFF) / UINT16_MAX;
}

uint16_t Fixed1616AsInt(Fixed1616* fixed) {
	return fixed->full >> 16;
}

void Fixed1616AddInt(Fixed1616* fixed, uint16_t amount) {
	fixed->full += ((uint32_t) amount) << 16;
}

void Fixed1616Add(Fixed1616* fixed, Fixed1616 amount) {
	fixed->full += amount.full;
}

void Fixed1616SubInt(Fixed1616* fixed, uint16_t amount) {
	fixed->full -= ((uint32_t) amount) << 16;
}

void Fixed1616Sub(Fixed1616* fixed, Fixed1616 amount) {
	fixed->full -= amount.full;
}

void Fixed1616MulInt(Fixed1616* fixed, uint16_t amount) {
	fixed->full *= amount;
}

void Fixed1616Mul(Fixed1616* fixed, Fixed1616 amount) {
	// If both numbers are greater than 1, multiplying them together will
	// overflow our available integral precision.  To fix this we chop off
	// 8 bits of fractional precision and do the multiplication then.
	if (fixed->full > (1 << 16) && amount.full > (1 << 16)) {
		fixed->full = (fixed->full >> 8) * (amount.full >> 8);
	} else {
		fixed->full *= amount.full;
		fixed->full >>= 16;
	}
}

void Fixed1616DivInt(Fixed1616* fixed, uint16_t amount) {
	fixed->full /= amount;
}

void Fixed1616Div(Fixed1616* fixed, Fixed1616 amount) {
	fixed->full /= amount.full;
	fixed->full <<= 16;
}
