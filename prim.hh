#pragma once

#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
using namespace glm;

struct aabb     { vec3 f,t; };
struct plane    { vec3 p,n; };
struct triangle { vec3 p,q,r; };
struct light    { vec3 p,c; float ia,id,is; };

struct ray {
  vec3 o,n,n1,d;
  float l;
  ray(vec3,vec3);
  void direct(float i,float j);
  bool hit(const aabb&,float&,float&);
  bool hit(const plane&,vec3&);
  bool hit(const triangle&,vec3&,vec3&);
};
