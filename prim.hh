#pragma once

#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
using namespace glm;

enum AXIS {X,Y,Z};

struct aabb     { dvec3 f,t; };
struct plane    { double e; AXIS k; };
struct triangle { dvec3 p,q,r; };
struct light    { dvec3 p,c; double ia,id,is; };

struct ray {
  dvec3 o,d,inv;
  dmat4x4 c2w;
  ray(dvec3,dvec3);
  ray(dvec3);
  void direct(double,double);
  void direct(dvec3&);
  bool hit(const aabb&,double&,double&);
  bool hit(triangle&,dvec3&,dvec3&);
};

void saturate(dvec3&I);
