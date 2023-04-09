#pragma once
#include "utils.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

class material {
  public: 
    virtual color emitted(float u, float v, point &p) {
      return color(0, 0, 0);
    }

    virtual bool scatter(
      ray &r_in, hit_record &rec, color &attenuation, ray& scattered  
    ) = 0;
};

class lambertian : public material {
  public :
    lambertian(color a) {
      albedo = make_shared<solid_color>(a);
    }
    lambertian(shared_ptr<texture> a) : albedo(a) {}
    
    virtual bool scatter (ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override {
      vec3 scatter_direction = rec.normal + random_unit_vector();
      if (near_zero(scatter_direction)) {
        scatter_direction = rec.normal;
      }
      scattered = ray(rec.p, scatter_direction);
      attenuation = albedo->value(rec.u, rec.v, rec.p);
      return true;
    }

  private:
    shared_ptr<texture> albedo;
};


class metal : public material {
  public :
    metal(color a, float f) {
      albedo.x = a.x;
      albedo.y = a.y;
      albedo.z = a.z;
      fuzz = f < 1 ? f : 1;
    }
    
    virtual bool scatter (ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override {
      vec3 reflected = reflect(glm::normalize(r_in.getDirection()), rec.normal);
      scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
      attenuation = albedo;
      return (glm::dot(scattered.getDirection(), rec.normal) > 0);
    }

  private:
    color albedo;
    float fuzz;
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

class diffuse_light : public material {
  public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color> (c)) {}

    virtual bool scatter(ray &r, hit_record &rec, point &p, ray &scattered) override {
      return false;
    }

    virtual color emitted(float u, float v, point &p) override {
      return emit->value(u, v, p);
    }

  private:
    shared_ptr<texture> emit;
};