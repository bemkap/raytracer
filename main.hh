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
  ray r={{0,3,-30},{0.00105257,0.00883711,0.99996}};
};
/*
Rayo 1: 
 {0,3,-30}
 {0.00105257,0.00883711,0.99996}
 {0.0264568,3.22213,-4.86548}

Rayo 2: 
 {19.4733,3.01545,16.592}
 {-0.754998,0.00343363,-0.655718}
 {3.85954,3.08646,3.03135}

Rayo 3: 
 {8.05917,-3.68279,2.01399}
 {-0.620148,0.714088,-0.3248}
 {4.17958,0.784479,-0.0179293}

Rayo 4:   
****** Ojo con este que se origina desde dentro de la BBox de la escena
 {-0.0039109,3.38799,0.408742}
 {0.299255,0.696669,0.651996}
 {1.16409,6.10711,2.95351}

Rayo 5: 
 {-2.73964,18.5392,3.71494}
 {0.222011,-0.919783,-0.323589}
 {-0.16381,7.86759,-0.0394237}
*/
