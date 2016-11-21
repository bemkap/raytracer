#pragma once

#include<glm/vec3.hpp>
#include<vector>
#include<string>
#include"mtl.hh"
#include"prim.hh"
using namespace glm;
using namespace std;

struct face {
  mat*m;
  union { long a[9]; struct { long v0,t0,n0, v1,t1,n1, v2,t2,n2; };};
  long operator[](int);
};

class obj {public:
  state st;
  vector<dvec3> vs,vts,ns;
  vector<face> fs;
  map<string,mat*> mm;
  obj(string&); ~obj();
};

inline dvec3 f2v(obj*o,long i,int v){return o->vs[o->fs[i][3*(v%3)]-1];}
inline dvec3 f2n(obj*o,long i,int v){return o->ns[o->fs[i][2+3*(v%3)]-1];}
inline triangle f2t(obj*o,long i){return {f2v(o,i,0),f2v(o,i,1),f2v(o,i,2)};}
inline double min3(obj*o,long i,unsigned d){
  return std::min(f2v(o,i,0)[d%3],std::min(f2v(o,i,1)[d%3],f2v(o,i,2)[d%3]));
}
inline double max3(obj*o,long i,unsigned d){
  return std::max(f2v(o,i,0)[d%3],std::max(f2v(o,i,1)[d%3],f2v(o,i,2)[d%3]));
}
