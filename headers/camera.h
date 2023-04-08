#pragma once
#include "ray.h"

class camera {
  public: 
    camera(point lookfrom, point lookat, vec3 vup, float vfov, float aspectRatio) {
      float theta = degrees_to_radian(vfov);
      float h = tanf32(theta / 2);
      float viewportHeight = 2.0f * h;
      float viewportWidth = aspectRatio * viewportHeight;
      
      auto w = glm::normalize(lookfrom - lookat);
      auto u = glm::normalize(glm::cross(vup, w));
      auto v = glm::cross(w, u);


      origin = lookfrom;
      horizontal = viewportWidth * u;
      vertical = viewportHeight * v;
      llc = origin - (0.5f * horizontal) - (0.5f * vertical) - w;
    }

    ray get_ray(float s, float t) {
      return ray(origin, llc + s * horizontal + t * vertical - origin);
    }
  private:
    point origin;
    point llc;
    vec3 horizontal;
    vec3 vertical;
};