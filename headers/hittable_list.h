#pragma once

#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
  public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) {
      add(object);
    }
    void add(shared_ptr<hittable> object) {
      objects.push_back(object);
    }
    void clear() {
      objects.clear();
    }
    virtual bool hit(ray &r, float t_min, float t_max, hit_record &rec) override;

  private:
    std::vector<shared_ptr<hittable>> objects;

};

bool hittable_list::hit(ray &r, float t_min, float t_max, hit_record &rec) {
  hit_record temp_rec;
  bool hit_anything = false;
  float closest_so_far = t_max;


  for (auto &object: objects) {
    if (object->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
  
}