#ifndef __INTERPOLATOR_H__
#define __INTERPOLATOR_H__

#include <stdint.h>
#include "easing_functions.h"

typedef struct Interpolator {
  // The start and end position for interpolating between.
  float start;
  float end;

  // A cached value of the distance between start and end.
  float span;

  // How many samples the interpolation should take.
  uint32_t duration_samples;
  uint32_t sample_count;

  EasingFunction func;
} Interpolator;

void Interpolator_Init(Interpolator* i,
                       float start, float end, float duration_seconds,
                       EasingFunction func);

float Interpolator_Get(Interpolator* i);

#endif
