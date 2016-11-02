#pragma once

#include<iostream>
#include<SDL2/SDL.h>
#include<glm/vec3.hpp>
#include<glm/ext.hpp>
#include"parser.hh"
#include"kdtree.hh"
using namespace std;
using namespace glm;

struct light { vec3 p,c; };
