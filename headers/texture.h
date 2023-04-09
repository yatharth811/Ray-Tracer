#pragma once
#include "utils.h"

class texture {
  public:
    virtual color value(float u, float v, point &p) = 0;
};

class solid_color : public texture {
  public:
    solid_color() {}
    solid_color(color c) : color_value(c) {}
    solid_color(float red, float green, float blue) : color_value(color(red, green, blue)) {}
    virtual color value(float u, float v, vec3 &p) override {
      return color_value;
    }
  private:
    color color_value;
};