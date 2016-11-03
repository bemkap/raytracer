#pragma once

#include"prim.hh"
#include"parser.hh"
using namespace glm;

struct kdtree {
  long i,j;
  vec3 n,vt;
  unsigned depth;
  box bounds;
  kdtree*left,*right;
  kdtree(obj_desc*,box,unsigned,long,long);
  bool hit(obj_desc*,ray,vec3&,vec3&);
  ~kdtree();
};
