#pragma once

#include<glm/vec3.hpp>
#include"parser.hh"
using namespace glm;

struct box { vec3 f,t; };
struct ray { vec3 p,d; };
struct tri { vec3 p,q,r; };

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
