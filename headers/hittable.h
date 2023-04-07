#pragma once
#include "ray.h"

struct hit_record {
  point p;
  vec3 normal;
  float t;
  bool front_face;

  inline void set_face_normal(ray &r, vec3 &outward_normal) {
    front_face = (glm::dot(r.getDirection(), outward_normal) < 0);
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
  public:
    virtual bool hit(ray& r,float t_min, float t_max, hit_record &rec) = 0;
};