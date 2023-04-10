#pragma once
#include "ray.h"
#include "onb.h"
#include "utils.h"

class pdf {
  public:
    virtual ~pdf() {}
    virtual float value(vec3 direction) = 0;
    virtual vec3 generate() = 0;
};

class cosine_pdf : public pdf {
  public:
    cosine_pdf(vec3 &w) {uvw.build_from_w(w);}
  
    virtual float value(vec3 direction) override {
      auto cosine = glm::dot(glm::normalize(direction), uvw.w());
      return (cosine <= 0) ? 0 : cosine/pi;
    }

    virtual vec3 generate() override {
      return uvw.local(random_cosine_direction());
    }

  public:
    onb uvw;
};