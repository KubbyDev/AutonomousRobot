#include "Tools.h"

#include <Arduino.h>
#include <stdlib.h>

void clamp(int* x, int min, int max) {
  if(*x < min) *x = min;
  if(*x > max) *x = max;
}

void clampAngle(float* angle) {
    *angle = fmodf(*angle, 2*PI);
    if(*angle < 0)
        *angle += (2*PI);
}
