#pragma once

#include<vector>
#include"prim.hh"
#include"obj.hh"
using namespace std;
using namespace glm;

struct elem;

class kdtree {public:
  vector<size_t> ts;
  plane split;
  unsigned depth;
  aabb bounds;
  kdtree*left,*right;
  kdtree(obj*,aabb,unsigned,vector<size_t>&); ~kdtree();
  bool leaf_p();
  bool hit(obj*,ray&,dvec3&,dvec3&,vector<light>&,int);
};

struct elem {kdtree*node; double in,out;};
