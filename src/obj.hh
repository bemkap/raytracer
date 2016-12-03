#pragma once

#include<regex>
#include<vector>
#include"common.hh"
#include"mtl.hh"

class face {public:
  mat*m;
  union {size_t a[9]; struct {size_t v0,t0,n0, v1,t1,n1, v2,t2,n2;};};
  size_t operator[](int);
};

class obj {public:
  vector<dvec3> vts,ns,vs;
  bool has_vts,has_ns;
  map<string,mat*> mm;
  mat*cmtl;
  state st;
  vector<face> fs;
  obj(const string&); ~obj();
  double min3(size_t,AXIS);
  double max3(size_t,AXIS);
};
