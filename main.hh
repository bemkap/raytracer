#pragma once

#include<iostream>
#include<SDL2/SDL.h>
#define GLM_FORCE_RADIANS
#include<glm/vec3.hpp>
#include<glm/ext.hpp>
#include"parser.hh"
#include"kdtree.hh"
#include"sdl.hh"
using namespace std;
using namespace glm;

namespace conf {
  box b={{-10,-10,-10},{10,10,10}};
  light l[]={{{0,3,-30},{0,255,0}},{{7,12,-10},{0,0,255}}};
  plane scr={{0,0,0},{0,0,1}};
  ray r={{0,3,-30},{0,0,0}};
};
