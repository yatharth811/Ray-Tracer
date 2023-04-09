#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "headers/col781.h"
#include <omp.h>
using namespace std;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x << ' ' << v.y << ' ' << v.z;
}

color ray_color(ray& r, color &background, hittable &world, int depth) {

  // hit_record rec;

  // if (depth == 0) return color(0, 0, 0);
   
  // // if (world.hit(r, 0.001, infinity, rec)) {
  // //   // PART 1.1
  // //   // return 0.5f * (rec.normal + color(1, 1, 1));

  // //   point target = rec.p + rec.normal + random_unit_vector();
  // //   ray newRay(rec.p, target - rec.p);
  // //   return 0.5f * ray_color(newRay, world, depth - 1);
  // // }

  // if (world.hit(r, 0.0001, infinity, rec)) {
  //   ray scattered;
  //   color attenuation;
    
  //   if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
  //     return attenuation * ray_color(scattered, world, depth - 1);
  //   }

  //   return color(0, 0, 0);

  // }

  // vec3 unit_direction = glm::normalize(r.getDirection());
  // float t = 0.5*(unit_direction.y + 1.0f);
  // return (1.0f-t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);

  hit_record rec;
  if (depth == 0) return color(0, 0, 0);

  if (!world.hit(r, 0.001, infinity, rec)) {
    return background;
  }

  ray scattered;
  color attenuation;
  color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

  if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
    return emitted;
  }

  return  emitted + attenuation * ray_color(scattered, background, world, depth - 1);

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

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<box>(point(130, 0, 65), point(295, 165, 230), white));
    objects.add(make_shared<box>(point(265, 0, 295), point(430, 330, 460), white));

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
  // auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8));
  
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
  // hittable_list world;

  // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  // auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  // auto material_left   = make_shared<dielectric>(1.5);
  // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2));

  // world.add(make_shared<sphere>(point( 0.0, -100.5, -1.0), 100.0, material_ground));
  // world.add(make_shared<sphere>(point( 0.0,    0.0, -1.0),   0.5, material_center));
  // world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0),   0.5, material_left));
  // world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0), -0.45, material_left));
  // world.add(make_shared<sphere>(point( 1.0,    0.0, -1.0),   0.5, material_right));

  // camera cam(point(-2,2,1), point(0,0,-1), vec3(0,1,0), 90, aspectRatio);

  // Test End

  // World
  // hittable_list world;
  // auto difflight = make_shared<diffuse_light> (color(4, 4, 4));
  // world.add(make_shared<sphere>(point(0,-1000,0), 1000, material_center));
  // world.add(make_shared<sphere>(point(0,2,0), 2, material_center));
  // world.add(make_shared<sphere>(point(3.0,    5.0, 0),   1, difflight));
  // world.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

  // // Camera specifications
  // point lookfrom(26, 3, 6);
  // point lookat(0, 2, 0);
  // float vfov = 20.0f;
  // color background(0, 0, 0);
  // camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, aspectRatio);

  // Cornell Box
  // hittable_list world;
  // auto red   = make_shared<lambertian>(color(.65, .05, .05));
  // auto white = make_shared<lambertian>(color(.73, .73, .73));
  // auto green = make_shared<lambertian>(color(.12, .45, .15));
  // auto light = make_shared<diffuse_light>(color(7, 7, 7));
  // world.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  // world.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
  // // world.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
  // world.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
  // world.add(make_shared<box>(point(130, 0, 65), point(295, 165, 230), white));
  // world.add(make_shared<box>(point(265, 0, 295), point(430, 330, 460), white));;
  // world.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
  // world.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
  // world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
  // point lookfrom(278, 278, -800);
  // point lookat(278, 278, 0);
  // float vfov = 40.0f;
  // color background(0, 0, 0);
  // camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, aspectRatio);

  switch (0) {
    default:
    case 1:
      world = cornell_box();
      aspectRatio = 1.0f;
      frameWidth = 600;
      frameHeight = static_cast<int>(frameWidth / aspectRatio);
      samples = 100;
      background = color(0, 0, 0);
      lookfrom = point(278, 278, -800);
      lookat = point(278, 278, 0);
      vfov = 40.0;
      break;
  }  


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
