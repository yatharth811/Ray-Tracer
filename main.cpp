#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<glm/glm.hpp>
#include "ray.h"
using vec3 = glm::vec3;
using point = vec3;
using color = vec3;
using namespace std;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x << ' ' << v.y << ' ' << v.z;
}

bool hit_sphere(point &center, float radius, ray &r) {
  vec3 oc = r.getOrigin() - center;
  float a = glm::dot(r.getDirection(), r.getDirection());
  float b = 2.0f * glm::dot(oc, r.getDirection());
  float c = glm::dot(oc, oc) - radius * radius;
  float discriminant = b * b - 4 * a * c;
  return (discriminant > 0);
}

color ray_color(ray& r) {
  point center(0, 0, -1);
  if (hit_sphere(center, 0.5f, r)) {
    return color(1, 0, 0);
  }

  vec3 unit_direction = (r.getDirection() / (float) r.getDirection().length());
  float t = 0.5*(unit_direction.y + 1.0);
  return (1.0f-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}




SDL_Surface* framebuffer = NULL;
SDL_Window* window = NULL;
SDL_Surface* windowSurface = NULL;
bool quit = false;

void handleEvents() {
  SDL_Event e;
  while(SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      quit = true;
    }
  }
}



int main() {

  // Image Specifications
  const float aspectRatio = 16.0f/9;
  const int frameWidth = 640;
  const int frameHeight = static_cast<int>(frameWidth / aspectRatio);


  // Camera (will implement this as class later.)
  float viewportHeight = 2.0;
  float viewportWidth = aspectRatio * viewportHeight;
  float focalLength = 1.0;
  vec3 origin = vec3(0, 0, 0);
  vec3 horizontal = vec3(viewportWidth, 0, 0);
  vec3 vertical = vec3(0, viewportHeight, 0);
  vec3 llc = origin - (0.5f * horizontal) - (0.5f * vertical) - vec3(0, 0, focalLength);
  
  // SDL Intialization
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      throw SDL_GetError();
  } 
  else {
    window = SDL_CreateWindow("COL781", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, frameWidth, frameHeight, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      throw SDL_GetError();
    } 
    else {
      windowSurface = SDL_GetWindowSurface(window);
      framebuffer = SDL_CreateRGBSurface(0, frameWidth, frameHeight, 32, 0, 0, 0, 0);
    }
  }

  

  // Rendering
  Uint32 *pixels = (Uint32 *) framebuffer->pixels;

  for (int j = 0; j < frameHeight; j += 1) {
    for (int i = 0; i < frameWidth; i += 1) {
      auto u = i * 1.0f / (frameWidth - 1);
      auto v = j * 1.0f / (frameHeight - 1);
      ray r(origin, llc + u * horizontal + v * vertical - origin);
      color pixelColor = 255.99f * ray_color(r);
      pixels[i + frameWidth * (frameHeight - 1 - j)] = SDL_MapRGB(framebuffer->format, pixelColor.x, pixelColor.y, pixelColor.z);
    }
  }


  SDL_BlitScaled(framebuffer, NULL, windowSurface, NULL);
  SDL_UpdateWindowSurface(window);

  while(!quit) {
    handleEvents();
  }
  return 0;
}
