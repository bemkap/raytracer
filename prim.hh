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
  dvec3 o,n,n1,d;
  double l;
  ray(dvec3,dvec3);
  void direct(double i,double j);
  bool hit(const aabb&,double&,double&);
  bool hit(const plane&,dvec3&);
  bool hit(const triangle&,dvec3&,dvec3&);
};
