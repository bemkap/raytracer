#pragma once

#include<SDL2/SDL.h>
#include<glm/vec3.hpp>
using namespace glm;

constexpr int WIDTH=300,HEIGHT=300;

struct sdl {
  SDL_Window*wi;
  SDL_Renderer*re;
  SDL_PixelFormat fmt;
  double r;
  Uint32 pixmap[WIDTH*HEIGHT];
  sdl(); ~sdl();
  void clear();
  void draw();
  void set(int,int,dvec3);
  void wait_input(dvec3&,bool&,bool&);
  static void r2s(double,double,double&,double&,double,double);
};
