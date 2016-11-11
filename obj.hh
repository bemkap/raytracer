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
  long v0,t0,n0, v1,t1,n1, v2,t2,n2;
  long operator[](int);
};

class obj {public:
  state st;
  vector<dvec3> vs,vts,ns;
  vector<face> fs;
  map<string,mat*> mm;
  obj(string&); ~obj();
  dvec3 f2v(long,int);
  dvec3 f2n(long,int);
  triangle f2t(long);
  double min3(long,unsigned);
  double max3(long,unsigned);
};
