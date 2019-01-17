#include "oscillator.h"

#include "defines.h"

void Osc_Init(Oscillator* osc, uint16_t freq_hz, const uint16_t* wave_table) {
  osc->phase = 0;
  osc->wave_table = wave_table;
  Osc_SetFreq(osc, freq_hz);
}

void Osc_SetFreq(Oscillator* osc, uint16_t freq_hz) {
  osc->freq_hz = freq_hz;

  // Every time we increment our phase, we want to do so by:
  // f / SAMPLE_RATE * TableLength.
  // We store the increment as a 32 bit fixed point 16.16 sample.
  osc->incr = ((uint32_t) osc->freq_hz << 16) / SAMPLE_RATE << 8;
}

uint16_t Osc_GetValue(Oscillator* osc) {
  // Phase is implicitly modded by 2^32 by overflow.
  osc->phase += osc->incr;
  uint8_t index = osc->phase >> 16;
  uint32_t sample_a = osc->wave_table[index];
  uint32_t sample_b = osc->wave_table[index + 1];

  // LERP between the two samples.
  uint16_t frac = osc->phase & 0xFFFF;
  uint16_t sample_a_portion = (sample_a * (UINT16_MAX - frac)) >> 16;
  uint16_t sample_b_portion = (sample_b * frac) >> 16;

  return sample_a_portion + sample_b_portion;
}

