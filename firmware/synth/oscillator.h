#ifndef __OSCILLATOR_H__
#define __OSCILLATOR_H__

#include <stdint.h>

typedef struct Oscillator {
  // The desired frequency for this oscillator.
  uint16_t freq_hz;

  // The current phase.
  uint32_t phase;

  // The pre-computed amount to increment the phase each sample.
  uint32_t incr;

  // The wave table from which this oscillator samples.
  const uint16_t* wave_table;
} Oscillator;

// Initialize an oscillator at a specified frequency with an associated wave
// table.
void Osc_Init(Oscillator* osc, uint16_t freq_hz, const uint16_t* wave_table);

// Set the frequency of the oscillator.
void Osc_SetFreq(Oscillator* osc, uint16_t freq_hz);

// Get the value from the oscillator and increment its internal counter.
uint16_t Osc_GetValue(Oscillator* osc);

#endif  // __OSCILLATOR_H__
