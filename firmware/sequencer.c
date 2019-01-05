#include "sequencer.h"

#include "defines.h"

void Sequencer_Init(Sequencer* seq, uint16_t bpm, uint8_t num_steps) {
  seq->counter = 0;
  seq->index = 0;
  seq->pulse_width = 1;

  Sequencer_SetBPM(seq, bpm);

  for (int i = 0; i < MAX_SEQUENCER_SIZE; ++i) {
    Sequencer_SetStepValue(seq, i, 0);
  }

  seq->num_steps = num_steps;
}

void Sequencer_SetNumSteps(Sequencer* seq, uint8_t num_steps) {
  seq->num_steps = num_steps;
}

void Sequencer_SetStepValue(Sequencer* seq, uint8_t step, uint16_t value) {
  seq->steps[step] = value;
}

void Sequencer_SetBPM(Sequencer* seq, uint16_t bpm) {
  seq->bpm = bpm;

  seq->threshold = SAMPLE_RATE / (bpm / 60.0f);
}

void Sequencer_SetPulseWidth(Sequencer* seq, float seconds) {
  seq->pulse_width = seconds * SAMPLE_RATE;
}

uint16_t Sequencer_Get(Sequencer* seq) {
  if (seq->counter == seq->threshold) {
    seq->counter = 0;
    ++seq->index;
    if (seq->index == seq->num_steps) {
      seq->index = 0;
    }
  }

  ++seq->counter;

  if (seq->counter <= seq->pulse_width) {
    return seq->steps[seq->index];
  }

  return 0;
}

