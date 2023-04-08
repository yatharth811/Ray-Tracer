#pragma once
#include<glm/glm.hpp>
using vec3 = glm::vec3;
using point = vec3;
using color = vec3;
class ray {
  public:
    ray() {}
    
    ray(const point &_origin, const vec3 &_direction){
      origin = _origin;
      direction = _direction;
    }
    point getOrigin() {
      return origin;
    }
    vec3 getDirection(){
      return direction;
    }

    point at(float t){
      return origin + t * direction;   
    }

  private:
    point origin;
    vec3 direction;
};