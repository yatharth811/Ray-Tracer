#pragma once
#include "utils.h"
#include "ray.h"
#include "hittable.h"

class material {
  public: 
    virtual bool scatter(
      ray &r_in, hit_record &rec, color &attenuation, ray& scattered  
    ) = 0;
};

class lambertian : public material {
  public :
    lambertian(color a) {
      albedo.x = a.x;
      albedo.y = a.y;
      albedo.z = a.z;
    }
    
    virtual bool scatter (ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override {
      vec3 scatter_direction = rec.normal + random_unit_vector();
      if (near_zero(scatter_direction)) {
        scatter_direction = rec.normal;
      }
      scattered = ray(rec.p, scatter_direction);
      attenuation = albedo;
      return true;
    }

  private:
    color albedo;
};


class metal : public material {
  public :
    metal(color a) {
      albedo.x = a.x;
      albedo.y = a.y;
      albedo.z = a.z;
    }
    
    virtual bool scatter (ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override {
      vec3 reflected = reflect(glm::normalize(r_in.getDirection()), rec.normal);
      scattered = ray(rec.p, reflected);
      attenuation = albedo;
      return (glm::dot(reflected, rec.normal) > 0);
    }

  private:
    color albedo;
};


class dielectric : public material {
  public:
    dielectric(float ref_index) {
      ir = ref_index;
    }
    
    virtual bool scatter (ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override {
      attenuation = color(1.0, 1.0, 1.0);
      float refraction_ratio = rec.front_face ? 1.0f / ir : ir;
      vec3 unit_direction = glm::normalize(r_in.getDirection());
      float cos_theta = fminf32(glm::dot(-unit_direction, rec.normal), 1.0f);
      float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
      vec3 direction;
      if (refraction_ratio * sin_theta > 1.0f || reflectance(cos_theta, refraction_ratio) > random_in_zo()) {
        direction = reflect(unit_direction, rec.normal);
      }
      else {
        direction = refract(unit_direction, rec.normal, refraction_ratio);
      }
      scattered = ray(rec.p, direction);
      return true;
    }

  private:
    float ir; // refractive index

    // Schlick's Approximation
    static float reflectance(float cosine, float mu) {
      float r0 = (1 - mu) / (1 + mu);
      r0 *= r0;
      return r0 + (1 - r0) * (1 - cosine) * (1 - cosine) * (1 - cosine) * (1 - cosine) * (1 - cosine);
    }
};