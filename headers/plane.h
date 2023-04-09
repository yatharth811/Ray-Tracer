#pragma once

#include "ray.h"
#include "hittable.h"
#include "material.h"

// Infinite Plane
class plane : public hittable{
  public:
    plane(glm::vec3 p, glm::vec3 n) : pt(p), normal(n){}
    virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec) override;

  private:
    point pt, normal;
};

bool plane::hit(ray &r, float tmin, float tmax, hit_record &rec){
	glm::vec3 o = r.getOrigin(), dir = r.getDirection();
	if(glm::dot(dir,normal) == 0) return false;
	float t = glm::dot(pt-o,normal)/glm::dot(dir,normal);
	if(t<tmin || t>tmax) return false;
	glm::vec3 p = r.at(t);
	rec.t = t;
	rec.p = p;
	rec.set_face_normal(r, normal);
	return true;
}


class xy_rect : public hittable {
	public:
		xy_rect() {}
		xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, shared_ptr<material> mat) {
			x0 = _x0;
			x1 = _x1;
			y0 = _y0;
			y1 = _y1;
			k = _k;
			mp = mat;
		}

		virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec) override {
			float t = (k - r.getOrigin().z) / (r.getDirection().z);
			if (t < tmin || t > tmax) return false;
			float x = r.getOrigin().x + t*r.getDirection().x;
			float y = r.getOrigin().y + t*r.getDirection().y;

			if (x < x0 || x > x1 || y < y0 || y > y1)
					return false;

			rec.u = (x-x0)/(x1-x0);
			rec.v = (y-y0)/(y1-y0);
			rec.t = t;
			vec3 outward_normal = vec3(0, 0, 1);
			rec.set_face_normal(r, outward_normal);
			rec.mat_ptr = mp;
			rec.p = r.at(t);
			return true;
		}

	private:
		shared_ptr<material> mp;
		float x0, x1, y0, y1, k;

};