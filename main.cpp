#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "headers/col781.h"
using vec3 = glm::vec3;
using point = vec3;
using color = vec3;
using namespace std;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x << ' ' << v.y << ' ' << v.z;
}

color ray_color(ray& r, hittable &world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec)) {
    return 0.5f * (rec.normal + color(1, 1, 1));
  }

  vec3 unit_direction = glm::normalize(r.getDirection());
  float t = 0.5*(unit_direction.y + 1.0f);
  return (1.0f-t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
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

  // World
  hittable_list world;
  world.add(make_shared<sphere>(point(0,0,-2), 0.5));
  world.add(make_shared<sphere>(point(0,-101,-2), 100));

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
      color pixelColor = 255.99f * ray_color(r, world);
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
