#pragma once

#include<glm/vec3.hpp>
#include<vector>
#include<string>
#include"mtl.hh"
#include"prim.hh"
using namespace glm;
using namespace std;

struct face {
  union {
    long a[9];
    struct { long v0,t0,n0, v1,t1,n1, v2,t2,n2; };
  };
  long operator[](int);
};

class obj {public:
  state st;
  vector<vec3> vs,vts,ns;
  vector<face> fs;
  vector<mtl*> mtls;
  obj(string&);
  ~obj();
  vec3 f2v(long,int);
  vec3 f2n(long,int);
  triangle f2t(long);
  float min3(long,unsigned);
  float max3(long,unsigned);
};
