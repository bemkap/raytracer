#pragma once

#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
using namespace glm;

struct box      { vec3 f,t; };
struct plane    { vec3 p,n; };
struct triangle { vec3 p,q,r; };
struct light    { vec3 p,c; };

struct ray {
  vec3 o,d;
  bool hit(const box&,float&,float&);
  bool hit(const plane&,vec3&);
  bool hit(const triangle&,vec3&,vec3&);
};
