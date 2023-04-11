#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../headers/col781.h"
#include <omp.h>
using namespace std;


inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x << ' ' << v.y << ' ' << v.z;
}

point_light p(point(0,10,0), color(15,15,15));
// point_light q(point(100,100,100), color(7, 7, 7));
vector<point_light*> v{&p};


color ray_color(ray &r, color &background, hittable &world, int depth) {
  if (depth == 0) return color(0, 0, 0);

  hit_record rec;
  if (!world.hit(r, 0.001, infinity, rec)) {
    return background;
  }
  
  color temp(0, 0, 0);
  for(auto i: v){
    ray t(rec.p,i->position-rec.p);
    hit_record tmp;
    scatter_record srec;
    if(world.hit(t,0.001,infinity,tmp)) return color(0,0,0);
    rec.mat_ptr->scatter(t, tmp, srec);
    temp += srec.attenuation * i->radiance * abs(glm::dot(tmp.normal, t.getDirection())) / (glm::distance(rec.p, i->position)*glm::distance(rec.p, i->position));
  }
  // cout << temp << endl;
  return temp;
 
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
  float aspectRatio;
  int frameWidth;
  int frameHeight;
  int samples;
  int max_depth = 5;

  
  // World
  hittable_list world;
  auto material_ground1 = make_shared<lambertian>(color(1.0, 0.0, 0.0));
  auto material_ground2 = make_shared<lambertian>(color(0.0,1.0,0.0));
  world.add(make_shared<sphere>(point(0,0,-2), 1, material_ground1));
  world.add(make_shared<sphere>(point(0,-101,-2), 100, material_ground2));
  // world.add(make_shared<plane>(point(0, -1, 0), vec3(0, 1, 0)));
  // world.add(make_shared<box>(point(-0.8, -0.8, -1), point(-0.2, 0.4, -2), material_ground));
  
  // // Camera specifications
  point lookfrom(0, 0, 0);
  point lookat(0, 0, -1);
  float vfov = 90.0f;
  color background(0, 0, 0);
  aspectRatio = 16.0f/9;
  camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, aspectRatio);
  frameWidth = 600;
  frameHeight = static_cast<int> (frameWidth / aspectRatio);
  samples = 100;
 

  // // Camera specifications
  

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
  int numt = 8;

  #pragma omp parallel num_threads(numt)
  {
    int tid = omp_get_thread_num();
    for (int j = tid; j < frameHeight; j += numt) {
      for (int i = 0; i < frameWidth; i += 1) {
        color pixelColor(0, 0, 0);
        for (int s = 0; s < samples; s += 1) {
          auto u = (i + random_in_zo()) * 1.0f / (frameWidth - 1);
          auto v = (j + random_in_zo()) * 1.0f / (frameHeight - 1);
          ray r = cam.get_ray(u, v);
          pixelColor += ray_color(r, background, world, max_depth);
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
  }

  std::cout << "Done tracing!!" << std::endl;

  SDL_BlitScaled(framebuffer, NULL, windowSurface, NULL);
  SDL_UpdateWindowSurface(window);

  while(!quit) {
    handleEvents();
  }
  return 0;
}

