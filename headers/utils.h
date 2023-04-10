#pragma once

#include <float.h>
#include <memory>
#include <random>
#include "ray.h"
#include <glm/glm.hpp>

using std::shared_ptr;
using std::make_shared;

// constants
const float infinity = FLT_MAX;
const float pi = 3.14159265f;

inline double degrees_to_radian(float degrees) {
  return degrees * pi / 180.0f;
}

inline float random_float(float min, float max) {
  static std::uniform_real_distribution<float> distribution(min, max);
  static std::mt19937 generator;
  return distribution(generator);
} 

inline vec3 random_vec3(float min, float max) {
  return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}


vec3 random_in_unit_sphere() {
  static std::uniform_real_distribution<float> distribution(-1, 1);
  static std::mt19937 generator;
  while(true){
    auto p = vec3(distribution(generator), distribution(generator), distribution(generator));
    if (glm::dot(p, p) >= 1) continue;
    return p;
  }
}

vec3 random_unit_vector() {
  return glm::normalize(random_in_unit_sphere());
}

static std::uniform_real_distribution<float> zo_distribution(0, 1);
static std::mt19937 zo_generator;

float random_in_zo(){
  return zo_distribution(zo_generator);
}

bool near_zero(vec3 &xyz) {
  return ((fabs(xyz.x) < 1e-8) & (fabs(xyz.y) < 1e-8) & (fabs(xyz.z) < 1e-8));
}

vec3 reflect(vec3 &v, vec3 &n) {
  return v - (glm::dot(v, n) * 2.0f * n);
}

vec3 refract(vec3 &v, vec3 &n, float nibynr) {
  float costheta = fminf32(glm::dot(-v, n), 1.0f);
  vec3 rout_perp = nibynr * (v + costheta * n);
  vec3 rout_para = -1.0f * sqrtf(fabsf32(1.0f - glm::dot(rout_perp, rout_perp))) * n;
  return rout_perp + rout_para;
}


inline vec3 random_cosine_direction() {
  auto r1 = random_float(0, 0.99999);
  auto r2 = random_float(0, 0.99999);
  auto z = sqrt(1-r2);

  auto phi = 2*pi*r1;
  auto x = cos(phi)*sqrt(r2);
  auto y = sin(phi)*sqrt(r2);

  return vec3(x, y, z);
}