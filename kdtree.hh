#pragma once

#include<stack>
#include<vector>
#include"prim.hh"
#include"parser.hh"
using namespace std;
using namespace glm;

struct elem;

class kdtree {
  stack<elem> trav;
public:
  vector<long> ts;
  plane split;
  unsigned depth;
  box bounds;
  kdtree*left,*right;
  kdtree(obj_desc*,box,unsigned,vector<long>&);
  ~kdtree();
  bool hit(obj_desc*,ray,vec3&,vec3&);
  bool leafp();
};

struct elem {
  kdtree*node;
  float in,out;
};
