#pragma once

#include<algorithm>
#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
#include<iostream>
#include<SDL2/SDL.h>
#include"kdtree.hh"
#include"parser.hh"
#include"sdl.hh"
using namespace std;
using namespace glm;

namespace cfg {
  box b={{-10,-10,-10},{10,10,10}};
  light l[]={{{0,3,-30},{0,255,0}},{{7,12,-10},{0,0,255}}};
  plane scr={{0,0,0},{0,0,1}};
  ray r={{0.446948,4.60535,-6.06405},{-0.25739,-0.62013,0.741073}};
};
/*
  {{0,3,-30},{0.00105257,0.00883711,0.99996}} {0.0264568,3.22213,-4.86548}
  {{19.4733,3.01545,16.592},{-0.754998,0.00343363,-0.655718}} {3.85954,3.08646,3.03135}
  {{8.05917,-3.68279,2.01399},{-0.620148,0.714088,-0.3248}} {4.17958,0.784479,-0.0179293}
  {{-0.0039109,3.38799,0.408742},{0.299255,0.696669,0.651996}} {1.16409,6.10711,2.95351}
  {{-2.73964,18.5392,3.71494},{0.222011,-0.919783,-0.323589}} {-0.16381,7.86759,-0.0394237}
*/
/*
  {{0.446948,4.60535,-6.06405},{-0.25739,-0.62013,0.741073}} {-1.46454,0,-0.560529}
  {{4.26472,4.60535,8.74243},{-0.149237,-0.594562,-0.790079}} {3.10876,0,2.62265}
  {{-13.0932,20.4824,-3.44963},{0.451476,-0.862177,0.229828}} {-2.36763,0,2.01031}
  {{-7.73158,-3.80066,10.3647},{0.557156,0.324706,-0.764293}} {-1.2101,0,1.41866}
  {{-2.43779,-3.05531,8.50044},{0.585132,0.323855,-0.743464}} {3.08246,0,1.48646}
*/
