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
  light l[]={{{0,3,-30},{0,255,0}},{{7,12,-10},{0,0,255}}};
  plane scr={{0,0,0},{0,0,1}};
  ray r={{0.446948,4.60535,-6.06405},{-0.25739,-0.62013,0.741073}};
  //{-1.46454,0,-0.560529}
};
