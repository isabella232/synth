#include "oscillator.h"

#include "defines.h"

void OscInit(Oscillator* osc, uint16_t freq_hz, const uint16_t* wave_table) {
	osc->phase = 0;
	osc->wave_table = wave_table;
	OscSetFreq(osc, freq_hz);
}

void OscSetFreq(Oscillator* osc, uint16_t freq_hz) {
	osc->freq_hz = freq_hz;

	// Every time we increment our phase, we want to do so by:
	// f / SAMPLE_RATE * TableLength.
	// We store the increment as a 32 bit fixed point 16.16 sample.
	osc->incr = ((osc->freq_hz << 8) << 16) / SAMPLE_RATE;
}

uint16_t OscGetValue(Oscillator* osc) {
	// Phase is implicitly modded by 2^32 by overflow.
	osc->phase += osc->incr;
	return osc->wave_table[(osc->phase >> 16) & 0xFF];
}

