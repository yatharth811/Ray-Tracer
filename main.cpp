#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "headers/col781.h"
using namespace std;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x << ' ' << v.y << ' ' << v.z;
}

color ray_color(ray& r, hittable &world, int depth) {

  hit_record rec;

  if (depth == 0) return color(0, 0, 0);
   
  // if (world.hit(r, 0.001, infinity, rec)) {
  //   // PART 1.1
  //   // return 0.5f * (rec.normal + color(1, 1, 1));

  //   point target = rec.p + rec.normal + random_unit_vector();
  //   ray newRay(rec.p, target - rec.p);
  //   return 0.5f * ray_color(newRay, world, depth - 1);
  // }

  if (world.hit(r, 0.0001, infinity, rec)) {
    ray scattered;
    color attenuation;
    
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * ray_color(scattered, world, depth - 1);
    }

    return color(0, 0, 0);

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
  const int samples = 10;
  const int max_depth = 5;

  // World
  // hittable_list world;
  // // world.add(make_shared<sphere>(point(0,0,-2), 0.5));
  // // world.add(make_shared<sphere>(point(0,-100.5,-1), 100));
  // // world.add(make_shared<plane>(point(0, -1, 0), vec3(0, 1, 0)));
  // // world.add(make_shared<box>(point(-0.8, -0.8, -1), point(-0.2, 0.4, -2)));

  // // TEST BEGIN
  // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  // // metal & diffuse
  // auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
  // // auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8));
  
  // //glass
  // // auto material_center = make_shared<dielectric>(1.5);
  // auto material_left   = make_shared<dielectric>(1.5);

  // // hollow glass sphere

  // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2));

  // world.add(make_shared<sphere>(point( 0.0, -100.5, -1.0), 100.0, material_ground));
  // world.add(make_shared<sphere>(point( 0.0,    0.0, -1.0),   0.5, material_center));
  // world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0),   0.5, material_left));
  // world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0),  -0.4, material_left));
  // world.add(make_shared<sphere>(point( 1.0,    0.0, -1.0),   0.5, material_right));
  // // TEST END
  // Camera (will implement this as class later.)
  // camera cam(90, aspectRatio);

  // Testing New Camera
  hittable_list world;

  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left   = make_shared<dielectric>(1.5);
  auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2));

  world.add(make_shared<sphere>(point( 0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point( 0.0,    0.0, -1.0),   0.5, material_center));
  world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0),   0.5, material_left));
  world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0), -0.45, material_left));
  world.add(make_shared<sphere>(point( 1.0,    0.0, -1.0),   0.5, material_right));

  camera cam(point(-2,2,1), point(0,0,-1), vec3(0,1,0), 90, aspectRatio);

  // Test End




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
      color pixelColor(0, 0, 0);
      for (int s = 0; s < samples; s += 1) {
        auto u = (i + random_in_zo()) * 1.0f / (frameWidth - 1);
        auto v = (j + random_in_zo()) * 1.0f / (frameHeight - 1);
        ray r = cam.get_ray(u, v);
        pixelColor += ray_color(r, world, 50);
      }
      pixelColor /= (1.0f * samples);

      // Gamma Correction
      pixelColor.r = sqrt(pixelColor.r);
      pixelColor.g = sqrt(pixelColor.g);
      pixelColor.b = sqrt(pixelColor.b);

      pixelColor *= 255.99f;
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
