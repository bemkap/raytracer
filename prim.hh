#pragma once

#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
using namespace glm;

struct aabb     { dvec3 f,t; };
struct plane    { dvec3 p,n; };
struct triangle { dvec3 p,q,r; };
struct light    { dvec3 p,c; double ia,id,is; };

struct ray {
  dvec3 o,d;
  double fov;
  dmat4x4 c2w;
  ray(dvec3,double,dvec3);
  ray(dvec3,double);
  void direct(double i,double j);
  bool hit(const aabb&,double&,double&);
  bool hit(const plane&,dvec3&);
  bool hit(const triangle&,dvec3&,dvec3&);
};
