#pragma once

#include "ray.h"
#include "hittable.h"

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


// XY Plane
class xy_plane : public hittable{
    public:
        xy_plane(float _x1, float _y1, float _x2, float _y2,float _k)
            : x1(_x1), y1(_y1), x2(_x2), y2(_y2), k(_k) 
        {}

        virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec) override;

    public:
        float x1,y1,x2,y2,k;
};

bool xy_plane::hit(ray &r, float tmin, float tmax, hit_record &rec) {
    glm::vec3 o = r.getOrigin(), dir = r.getDirection();
    float t = (k-o.z)/dir.z;
    if(t<tmin || t>tmax) return false;
    glm::vec3 p = r.at(t);
    if(p.x<x1 || p.x>x2 || p.y<y1 || p.y>y2) return false;
    rec.t = t;
    rec.p = p;
    vec3 zz(0, 0, 1);
    rec.set_face_normal(r, zz);
    return true;
}

// YZ Plane
class yz_plane : public hittable{
    public:
        yz_plane(float _y1, float _z1, float _y2, float _z2,float _k)
            : y1(_y1), z1(_z1), y2(_y2), z2(_z2), k(_k) 
        {}

        virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec) override;

    public:
        float y1,z1,y2,z2,k;
};

bool yz_plane::hit(ray &r, float tmin, float tmax, hit_record &rec) {
    glm::vec3 o = r.getOrigin(), dir = r.getDirection();
    float t = (k-o.x)/dir.x;
    if(t<tmin || t>tmax) return false;
    glm::vec3 p = r.at(t);
    if(p.z<z1 || p.z>z2 || p.y<y1 || p.y>y2) return false;
    rec.t = t;
    rec.p = p;
    vec3 xx(1, 0, 0);
    rec.set_face_normal(r, xx);
    return true;
}

// ZX Plane
class zx_plane : public hittable{
    public:
        zx_plane(float _z1, float _x1, float _z2, float _x2,float _k)
            : x1(_x1), z1(_z1), x2(_x2), z2(_z2), k(_k) 
        {}

        virtual bool hit(ray &r, float tmin, float tmax, hit_record &rec)  override;

    public:
        float x1,z1,x2,z2,k;
};

bool zx_plane::hit(ray &r, float tmin, float tmax, hit_record &rec) {
    glm::vec3 o = r.getOrigin(), dir = r.getDirection();
    float t = (k-o.y)/dir.y;
    if(t<tmin || t>tmax) return false;
    glm::vec3 p = r.at(t);
    if(p.x<x1 || p.x>x2 || p.z<z1 || p.y>z2) return false;
    rec.t = t;
    rec.p = p;
    vec3 yy(0, 1, 0);
    rec.set_face_normal(r, yy);
    return true;
}

