#pragma once

#include<algorithm>
#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
#include<iostream>
#include<SDL2/SDL.h>
#include"kdtree.hh"
#include"obj.hh"
#include"sdl.hh"
using namespace std;
using namespace glm;

namespace cfg {
  vector<light> ls;
  plane scr={{0,0,0},{0,0,1}};
  ray r={{0,3,-30},{0,0,0}};
};
