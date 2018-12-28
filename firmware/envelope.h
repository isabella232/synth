#ifndef __ENVELOPE_H__
#define __ENVELOPE_H__

#include <stdint.h>
#include "fixed.h"

typedef enum EnvelopeState {
	ENVELOPE_IDLE,
	ENVELOPE_ATTACK,
	ENVELOPE_DECAY,
	ENVELOPE_SUSTAIN,
	ENVELOPE_RELEASE,
} EnvelopeState;

typedef struct Envelope {
	// The length in samples of each phase of the envelope.
	uint16_t attack_samples;
	uint16_t decay_samples;
	uint16_t release_samples;

	// When modulating, how big of a step to take each time.
	Fixed1616 attack_step;
	Fixed1616 decay_step;
	Fixed1616 release_step;

	uint16_t sustain_level;

	EnvelopeState state;

	uint16_t counter;
} Envelope;

// Initializes the Envelope with attack, decay, and release all in seconds.
// sustain is measured in volume.
void EnvelopeInit(Envelope* e,
		float attack_seconds, float decay_seconds, uint16_t sustain_level, float release_seconds);

// Triggers the envelope.
void EnvelopeTrigger(Envelope* e);

// Releases the envelope.
void EnvelopeRelease(Envelope* e);

// Processes the given input sample according to the envelope state.
uint16_t EnvelopeProcess(Envelope* e, uint16_t input);

#endif  // __ENVELOPE_H__
