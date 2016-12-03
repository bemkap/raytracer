#pragma once

#include"common.hh"
#include"obj.hh"
using namespace glm;

struct ray {
  dvec3 o,d,inv;
  dmat4x4 c2w;
  ray(const dvec3&,const dvec3&);
  ray(const dvec3&);
  ray(){}
  void direct(double,double);
  void direct(const dvec3&);
  bool hit(const aabb&,double&,double&);
  bool hit(obj*,size_t,dvec3&,dvec3&);
};

void saturate(dvec3&);
