#pragma once
#include "ray.h"

class point_light {
  public:
    point_light() {}
    point_light (point pos, color rad) {
      position = pos;
      radiance = rad;
    }

  public:
    point position;
    color radiance;
};