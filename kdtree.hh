#pragma once

#include<glm/vec3.hpp>
#include"parser.hh"
using namespace glm;

struct box { vec3 f,t; };
struct ray { vec3 p,d; };
struct tri { vec3 p,q,r; };

struct kdtree {public:
  long i,j;
  vec3 n,vt;
  unsigned depth;
  box bounds;
  kdtree*left,*right;
  kdtree(obj_desc&,box,unsigned,long,long);
  kdtree(obj// _desc&,box);
  bool hit(obj_desc&,ray,vec3&,vec3&);
  ~kdtree();
};
