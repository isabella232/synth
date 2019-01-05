#ifndef __SEQUENCER_H__
#define __SEQUENCER_H__

#include <stdint.h>

#include "defines.h"

typedef struct Sequencer {
  uint16_t bpm;
  uint16_t steps[MAX_SEQUENCER_SIZE];
  uint8_t num_steps;
  uint32_t pulse_width;

  uint32_t threshold;
  uint32_t counter;
  uint32_t index;
} Sequencer;

void Sequencer_Init(Sequencer* seq, uint16_t bpm, uint8_t num_steps);

void Sequencer_SetNumSteps(Sequencer* seq, uint8_t num_steps);
void Sequencer_SetStepValue(Sequencer* seq, uint8_t step, uint16_t value);
void Sequencer_SetBPM(Sequencer* seq, uint16_t bpm);
void Sequencer_SetPulseWidth(Sequencer* seq, float seconds);

uint16_t Sequencer_Get(Sequencer* seq);

#endif  // __SEQUENCER_H__
