#pragma once

#include<glm/vec3.hpp>
#include<vector>
#include<string>
#include"mtl.hh"
#include"prim.hh"
using namespace glm;
using namespace std;

enum VERT {V0=0,V1=3,V2=6};
enum TEXT {T0=1,T1=4,T2=7};
enum NORM {N0=2,N1=5,N2=8};

class face {public:
  mat*m;
  union { size_t a[9]; struct { size_t v0,t0,n0, v1,t1,n1, v2,t2,n2; };};
  size_t operator[](int);
};

class obj {
  vector<dvec3> vts,ns;
public:
  state st;
  vector<dvec3> vs;
  vector<face> fs;
  map<string,mat*> mm;
  obj(string&); ~obj();
  double get_vert(size_t,VERT,AXIS);
  dvec3 get_vert(size_t,VERT);
  double get_norm(size_t,NORM,AXIS);
  dvec3 get_norm(size_t,NORM);
  double min3(size_t,AXIS);
  double max3(size_t,AXIS);
  triangle get_tri(size_t);
  void stats();
};
