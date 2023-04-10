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

class translate : public hittable {
  public:
    translate(shared_ptr<hittable> p, vec3 displacement) {
      ptr = p;
      offset = displacement;
    }

    virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec) override {
      ray moved(r.getOrigin() - offset, r.getDirection());
      if (!ptr->hit(moved, tmin, tmax, rec)) return false;

      rec.p += offset;
      rec.set_face_normal(moved, rec.normal);
      return true;
    }

  private:
    shared_ptr<hittable> ptr;
    vec3 offset;
};

class rotate_y : public hittable {
  public: 
    rotate_y(shared_ptr<hittable> p, float angle) {
      ptr = p;
      float radians = degrees_to_radian(angle);
      sin_theta = sin(radians);
      cos_theta = cos(radians);
    }

    virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec) override {
      auto origin = r.getOrigin();
      auto direction = r.getDirection();

      origin.x = cos_theta * r.getOrigin().x - sin_theta * r.getOrigin().z;
      origin.z = sin_theta * r.getOrigin().x + cos_theta * r.getOrigin().z;
      direction.x = cos_theta * r.getDirection().x - sin_theta * r.getDirection().z;
      direction.z = sin_theta * r.getDirection().x + cos_theta * r.getDirection().z;
      ray rotated(origin, direction);

      if (!ptr->hit(rotated, tmin, tmax, rec)) {return false;}

      auto p = rec.p;
      auto normal = rec.normal;
      p.x =  cos_theta * rec.p.x + sin_theta * rec.p.z;
      p.z = -sin_theta * rec.p.x + cos_theta * rec.p.z;
      normal.x =  cos_theta * rec.normal.x + sin_theta * rec.normal.z;
      normal.z = -sin_theta * rec.normal.x + cos_theta * rec.normal.z;

      rec.p = p;
      rec.set_face_normal(rotated, normal);
      return true;

    }

  private:
    shared_ptr<hittable> ptr;
    float sin_theta;
    float cos_theta;
};


class flip_face : public hittable {
  public:
    flip_face(shared_ptr<hittable> p) : ptr(p) {}

    virtual bool hit(ray& r, float tmin, float tmax, hit_record& rec) override {
      if (!ptr->hit(r, tmin, tmax, rec))
          return false;

      rec.front_face = !rec.front_face;
      return true;
    }

  public:
    shared_ptr<hittable> ptr;
};