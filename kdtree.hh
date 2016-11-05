#pragma once

#include<vector>
#include"prim.hh"
#include"obj.hh"
using namespace std;
using namespace glm;

struct elem;

class kdtree {public:
  vector<long> ts;
  plane split;
  unsigned depth;
  aabb bounds;
  kdtree*left,*right;
  kdtree(obj*,aabb,unsigned,vector<long>&);
  ~kdtree();
  bool hit(obj*,ray,vec3&,vec3&);
  bool leafp();
};

struct elem {
  kdtree*node;
  float in,out;
};
