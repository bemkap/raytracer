#pragma once

#include<SDL2/SDL.h>
#include<glm/vec3.hpp>

constexpr int WIDTH=300,HEIGHT=300;

struct sdl {
  SDL_Window*wi;
  SDL_Renderer*re;
  SDL_PixelFormat fmt;
  Uint32 pixmap[WIDTH*HEIGHT];
  sdl();
  ~sdl();
  void draw();
  void set(int,int,glm::vec3);
  void waitexit();
};
