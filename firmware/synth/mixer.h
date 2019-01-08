#ifndef __MIXER_H__
#define __MIXER_H__

#include <stdint.h>

typedef struct Mixer {
  uint16_t gain_a, gain_b;
} Mixer;

// Sets the gain for a two channel mixer from 0 to 1.
void MixerSetGain(Mixer* mixer, float a, float b);

// Returns the mixed value of the two inputs.
uint16_t MixerMix(Mixer* mixer, uint16_t a, uint16_t b);

#endif  // __MIXER_H__
