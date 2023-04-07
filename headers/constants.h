#pragma once

#include <float.h>
#include <memory>

using std::shared_ptr;
using std::make_shared;

// constants
const float infinity = FLT_MAX;
const float pi = 3.14159265f;

inline double degrees_to_radian(float degrees) {
  return degrees * pi / 180.0f;
}

#include "ray.h"