#include "interpolator.h"

#include "defines.h"

void Interpolator_Init(Interpolator* i,
                       float start, float end, float duration_seconds,
                       EasingFunction func) {
  i->start = start;
  i->end = end;
  i->span = end - start;
  i->duration_samples = duration_seconds * SAMPLE_RATE;
  i->sample_count = 0;

  i->func = func;
}

float Interpolator_Get(Interpolator* i) {
  if (i->sample_count > i->duration_samples) {
    return i->end;
  }

  // Compute how far along we are, and then ease it according to the specified
  // function.
  float progress = (float) i->sample_count++ / i->duration_samples;
  progress = i->func(progress);

  // Return the eased position.
  return i->start + i->span * progress;
}
