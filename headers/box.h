#pragma once

#include "ray.h"
#include "hittable_list.h"
#include "plane.h"

// Axis Aligned Box

class box: public hittable {
  public:
    box(glm::vec3 _p1, glm::vec3 _p2);
    virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec) override;

  private:
    glm::vec3 p1,p2;
    hittable_list rects;
};

box::box(glm::vec3 _p1, glm::vec3 _p2){
    p1 = _p1;
    p2 = _p2;
    rects.add(make_shared<xy_plane>(p1.x, p1.y, p2.x, p2.y, p2.z));
    rects.add(make_shared<xy_plane>(p1.x, p1.y, p2.x, p2.y, p1.z));
    rects.add(make_shared<yz_plane>(p1.y, p1.z, p2.y, p2.z, p2.x));
    rects.add(make_shared<yz_plane>(p1.y, p1.z, p2.y, p2.z, p1.x));
    rects.add(make_shared<zx_plane>(p1.z, p1.x, p2.z, p2.x, p1.y));
    rects.add(make_shared<zx_plane>(p1.z, p1.x, p2.z, p2.x, p2.y));
}

bool box::hit(ray &r, float tmin, float tmax, hit_record &rec){
    return rects.hit(r,tmin,tmax,rec);
}