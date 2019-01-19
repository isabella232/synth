#ifndef __EASING_FUNCTIONS_H__
#define __EASING_FUNCTIONS_H__

#include <math.h>

// All easing functions expect input between 0 and 1, and return values between
// 0 and 1.

typedef float (*EasingFunction)(float);

float LinearEase(float x) {
  return x;
}

float QuadraticEaseIn(float x) {
  return x * x;
}

float QuadraticEaseOut(float x) {
  return 1.0f - QuadraticEaseIn(1.0f - x);
}

float QuadraticEaseInOut(float x) {
  if (x < 0.5f) {
    return QuadraticEaseIn(x * 2) / 2.0f;
  } else {
    return 0.5f + QuadraticEaseOut((x - 0.5f) * 2) / 2.0f;
  }
}

float ExponentialEaseIn(float x) {
  return pow(2, 10 * (x - 1));
}

float ExponentialEaseOut(float x) {
  return 1.0f - ExponentialEaseIn(1.0f - x);
}

float ExponentialEaseInOut(float x) {
  if (x < 0.5f) {
    return ExponentialEaseIn(x * 2) / 2.0f;
  } else {
    return 0.5f + ExponentialEaseOut((x - 0.5f) * 2) / 2.0f;
  }
}

#endif
