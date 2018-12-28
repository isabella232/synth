#include "envelope.h"

#include "defines.h"
#include "fixed.h"

#include <stdio.h>

void reset(Envelope* e) {
	e->state = ENVELOPE_IDLE;
	e->counter = 0;
}

void set_state(Envelope* e, EnvelopeState state) {
	e->state = state;
	e->counter = 0;
}

void EnvelopeInit(Envelope* e, float attack_seconds, float decay_seconds,
		uint16_t sustain_level, float release_seconds) {
	// Compute the number of samples for each phase of the envelope.
	e->attack_samples = attack_seconds * SAMPLE_RATE;
	e->decay_samples = decay_seconds * SAMPLE_RATE;
	e->release_samples = release_seconds * SAMPLE_RATE;

	// How big of a step size we take while attacking.  This gets us from 0
	// to 1 in attack_seconds.
	e->attack_step = Fixed1616FromInt(1);
	Fixed1616DivInt(&e->attack_step, e->attack_samples);

	// How big of a step to take each time to get us from 1 to sustain
	// level in decay_seconds.
	e->decay_step = Fixed1616FromInt(UINT16_MAX - sustain_level);
	Fixed1616DivInt(&e->decay_step, UINT16_MAX);
	Fixed1616DivInt(&e->decay_step, e->decay_samples);

	e->sustain_level = sustain_level;

	// How big of a step to take each time to get us from sustain level to 0.
	e->release_step = Fixed1616FromInt(sustain_level);
	Fixed1616DivInt(&e->release_step, UINT16_MAX);
	Fixed1616DivInt(&e->release_step, e->release_samples);

	reset(e);
}

void EnvelopeTrigger(Envelope* e) {
	set_state(e, ENVELOPE_ATTACK);
}

void EnvelopeRelease(Envelope* e) {
	set_state(e, ENVELOPE_RELEASE);
}

Fixed1616 envelope_amount(Envelope* e) {
	Fixed1616 f, f1;
	switch (e->state) {
		case ENVELOPE_IDLE:
			f = Fixed1616FromInt(0);
			break;
		case ENVELOPE_ATTACK:
			f = Fixed1616FromInt(e->counter);
			Fixed1616Mul(&f, e->attack_step);
			break;
		case ENVELOPE_DECAY:
			f = Fixed1616FromInt(1);
			f1 = Fixed1616FromInt(e->counter);
			Fixed1616Mul(&f1, e->decay_step);
			Fixed1616Sub(&f, f1);
			break;
		case ENVELOPE_SUSTAIN:
			f = Fixed1616FromInt(e->sustain_level);
			break;
		case ENVELOPE_RELEASE:
			f = Fixed1616FromInt(e->release_samples - e->counter);
			Fixed1616Mul(&f, e->release_step);
			break;
	}

	return f;
}

uint16_t EnvelopeProcess(Envelope* e, uint16_t input) {
	++e->counter;

	switch (e->state) {
		case ENVELOPE_IDLE:
			return 0;
		case ENVELOPE_ATTACK:
			// If we're at our maximum attack amount, switch to decay.
			if (e->counter == e->attack_samples) {
				set_state(e, ENVELOPE_DECAY);
				return input;
			}
			break;
		case ENVELOPE_DECAY:
			// If we're at max decay, switch to sustain.
			if (e->counter == e->decay_samples) {
				set_state(e, ENVELOPE_SUSTAIN);
				return e->sustain_level;
			}
			break;
		case ENVELOPE_SUSTAIN:
			return e->sustain_level;
		case ENVELOPE_RELEASE:
			// If we're at max release, switch to idle.
			if (e->counter == e->release_samples) {
				set_state(e, ENVELOPE_IDLE);
				return 0;
			}
			break;
	}

	// Return an interpolated value from 0 to input.
	Fixed1616 interp = envelope_amount(e);
	Fixed1616MulInt(&interp, input);
	return Fixed1616AsInt(&interp);
}
