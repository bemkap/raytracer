#pragma once

#include<vector>
#include"prim.hh"
#include"obj.hh"
using namespace std;
using namespace glm;

class kdtree {public:
  vector<long> ts;
  plane split;
  unsigned depth;
  aabb bounds;
  kdtree*left,*right;
  kdtree(obj*,aabb,unsigned,vector<long>&); ~kdtree();
  bool leafp();
  bool hit(obj*,ray,dvec3&,vector<light>&ls,int);
};

struct elem {
  kdtree*node;
  double in,out;
};
