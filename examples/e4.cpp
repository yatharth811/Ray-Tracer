#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../headers/col781.h"
#include <omp.h>
#include <cstdlib>
using namespace std;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x << ' ' << v.y << ' ' << v.z;
}


float prob = 0.8f;
// Final ray_color
color ray_color(ray &r, color &background, hittable &world, int depth) {
  if (depth == 0) return color(0, 0, 0);
  // std::srand(std::time(nullptr));
  float rand_num = ((float) rand() / (RAND_MAX));
  if(rand_num>prob){
    return color(0,0,0);
  }
  hit_record rec;
  if (!world.hit(r, 0.001, infinity, rec)) {
    return background;
  }

  scatter_record srec;
  color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
  if (!rec.mat_ptr->scatter(r, rec, srec)) {
    return emitted;
  }

  if (srec.is_specular) {
    return srec.attenuation
      * ray_color(srec.specular_ray, background, world, depth-1);
  }

  cosine_pdf p(rec.normal);
  ray scattered = ray(rec.p, p.generate());
  float pdf_val = p.value(scattered.getDirection());
  return emitted + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered) * ray_color(scattered, background, world, depth - 1) / (pdf_val*prob);


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


hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    // objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    // objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    // 2 sided light
    // objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));

    // unidirectional light
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));

    objects.add(make_shared<xz_rect>(-1000, 1000, -1000, 1000, 0, green));
    // objects.add(make_shared<xz_rect>(-1000, -1000, 1000, 1000, 0, green));

    // objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    // objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    // objects.add(make_shared<box>(point(130, 0, 65), point(295, 165, 230), white));
    // objects.add(make_shared<box>(point(265, 0, 295), point(430, 330, 460), white));

    // Rotated instead.
    shared_ptr<hittable> box1 = make_shared<box>(point(0, 0, 0), point(160, 160, 160), white);
    box1 = make_shared<rotate_y>(box1, 15);
    // box1 = make_shared<translate>(box1, vec3(265,0,295));
    box1 = make_shared<translate>(box1, vec3(400,0,400));

    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(point(0,0,0), point(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    // box2 = make_shared<translate>(box2, vec3(130,0,65));
    box2 = make_shared<translate>(box2, vec3(0,0,100));

    objects.add(box2);

    // Checking specular
    

    auto glass = make_shared<dielectric>(1.5);
    // objects.add(make_shared<sphere>(point(190,90,190), 90 , white));
    objects.add(make_shared<sphere>(point(250,120,220), 90 , white));

    return objects;
}

hittable_list final_scene() {
  // hittable_list boxes1;
  hittable_list objects;
  auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));
  const int boxes_per_side = 20;
  for (int i = 0; i < boxes_per_side; i++) {
    for (int j = 0; j < boxes_per_side; j++) {
      auto w = 100.0;
      auto x0 = -1000.0 + i*w;
      auto z0 = -1000.0 + j*w;
      auto y0 = 0.0;
      auto x1 = x0 + w;
      auto y1 = random_float(1,101);
      auto z1 = z0 + w;
      objects.add(make_shared<box>(point(x0,y0,z0), point(x1,y1,z1), ground));
    }
  }

  auto light = make_shared<diffuse_light>(color(15, 15, 15));
  objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));
  objects.add(make_shared<sphere>(point(260, 150, 45), 50, make_shared<dielectric>(1.5)));
  objects.add(make_shared<sphere>(
      point(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1)
  ));

  // have to remove this one
  objects.add(make_shared<sphere>(point(220,280,300), 80, make_shared<lambertian>(color(0.7, 0.3, 0.3))));

  hittable_list boxes2;
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  int ns = 100;
  for (int j = 0; j < ns; j++) {
      objects.add(
        make_shared<translate>(
          make_shared<rotate_y>(
            make_shared<sphere>(random_vec3(0, 165), 10, white), 15
          ), vec3(-100,270,395)
        )
      );
  }

  return objects;

}

int main() {

  // Image Specifications
  float aspectRatio;
  int frameWidth;
  int frameHeight;
  int samples;
  int max_depth = 5;

  // World Specifications
  hittable_list world;
  point lookfrom;
  point lookat;
  float vfov;
  color background;

  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
  auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 1);
  auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1);

  // world.add(make_shared<sphere>(point( 0.0, -100.5, -1.0), 100.0, material_ground));
  // world.add(make_shared<sphere>(point( 0.0,    0.0, -1.0),   0.5, material_center));
  // shared_ptr<hittable> box1 = make_shared<box>(point(-0.5*5, -0.5*5, -2*5), point(0.5*5, 0.5*5, -10.0*5), material_center);
  // // box1 = make_shared<translate>(box1, vec3(0.0,0,6.0));
  // // box1 = make_shared<rotate_y>(box1, -12);
  // // box1 = make_shared<translate>(box1, vec3(-0.5,0,0.0));
  // world.add(box1);

  // world.add(make_shared<sphere>(point(-5.0,    0.0, -5.0),   2.5, material_left));
  // world.add(make_shared<sphere>(point( 5.0,    0.0, -5.0),   2.5, material_right));
  // auto light = make_shared<diffuse_light>(color(3, 3, 3));
  // world.add((make_shared<sphere>(point(100, 100, 1000), 500, light)));

  world = cornell_box();
  aspectRatio = 1.0f;
  frameWidth = 640;
  frameHeight = static_cast<int> (frameWidth / aspectRatio);
  samples = 1000;
  background = color(0, 0, 0);
  lookfrom = point(278, 278, -800);
  lookat = point(278, 278, 0);
  vfov = 40.0;

  camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, aspectRatio);

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