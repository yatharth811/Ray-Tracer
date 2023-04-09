#pragma once
#include "ray.h"
#include "utils.h"

class material;

struct hit_record {
  point p;
  vec3 normal;
  shared_ptr<material> mat_ptr;
  float t;
  // Surface coordinates
  float u;
  float v;
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