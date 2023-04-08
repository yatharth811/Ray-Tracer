#pragma once
#include "hittable.h"

class sphere : public hittable {
  public:
    sphere(point _center, float _radius, shared_ptr<material> m) {
      center = _center;
      radius = _radius;
      mat_ptr = m;
    }

    virtual bool hit(
      ray& r,
      float t_min,
      float t_max,
      hit_record &rec
    ) override;

  private:
    point center;
    float radius;
    shared_ptr<material> mat_ptr;
};

bool sphere::hit(ray &r, float t_min, float t_max, hit_record &rec){
  vec3 oc = r.getOrigin() - center;
  float a = glm::dot(r.getDirection(), r.getDirection());
  float b = glm::dot(oc, r.getDirection());
  float c = glm::dot(oc, oc) - radius * radius;
  float discriminant = b * b - a * c;

  if (discriminant < 0) return false;
  float sqrtd = sqrtf(discriminant);
  float root = (-b - sqrtd) / a;
  if (root < t_min || root > t_max) {
    root = (-b + sqrtd) / a;
    if (root < t_min || root > t_max) return false;
  }

  rec.t = root;
  rec.p = r.at(root);
  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr; 
  return true;
}