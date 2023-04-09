#pragma once

#include "ray.h"
#include "hittable_list.h"
#include "plane.h"

// Axis Aligned Box

class box: public hittable {
  public:
    box() {}
    box(point p0, point p1, shared_ptr<material> ptr);
    virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec) override;

  private:
    point box_min;
    point box_max;
    hittable_list rects;
};

box::box(point p0, point p1, shared_ptr<material> ptr){
    box_min = p0;
    box_max = p1;
    rects.add(make_shared<xy_rect>(p0.x, p1.x, p0.y, p1.y, p1.z, ptr));
    rects.add(make_shared<xy_rect>(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));
    rects.add(make_shared<xz_rect>(p0.x, p1.x, p0.z, p1.z, p1.y, ptr));
    rects.add(make_shared<xz_rect>(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));
    rects.add(make_shared<yz_rect>(p0.y, p1.y, p0.z, p1.z, p1.x, ptr));
    rects.add(make_shared<yz_rect>(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
}

bool box::hit(ray &r, float tmin, float tmax, hit_record &rec){
    return rects.hit(r, tmin, tmax,rec);
}