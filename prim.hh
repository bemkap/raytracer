#pragma once

#define GLM_FORCE_RADIANS
#include<glm/vec3.hpp>
#include<glm/ext.hpp>
using namespace glm;

struct box      { vec3 f,t; };
struct plane    { vec3 p,n; };
struct triangle { vec3 p,q,r; };
struct light    { vec3 p,c; };

struct ray {
  vec3 p,d;
  bool hit(const box&);
  bool hit(const plane&,vec3&);
  bool hit(const triangle&,vec3&,vec3&);
};
