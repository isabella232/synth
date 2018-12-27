#include "mixer.h"

void MixerSetGain(Mixer* mixer, float a, float b) {
	mixer->gain_a = UINT16_MAX * a;	
	mixer->gain_b = UINT16_MAX * b;	
}

static uint16_t saturating_add(uint16_t a, uint16_t b) {
	uint16_t sum = a + b;
	if (sum < a) {
		return UINT16_MAX;
	}
	return sum;
}

uint16_t MixerMix(Mixer* mixer, uint16_t a, uint16_t b) {
	return saturating_add(
		((uint32_t) a * mixer->gain_a) >> 16,
		((uint32_t) b * mixer->gain_b) >> 16);
}
