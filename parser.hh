#pragma once

#include<glm/vec3.hpp>
#include<vector>
#include<iostream>
using namespace glm;
using namespace std;

struct face {
  union {
    long a[9];
    struct { long v0,t0,n0, v1,t1,n1, v2,t2,n2; };
  };
  long operator[](int);
};

struct mtl {
  string name;
  vec3 ka,kd,ks;//ambient,diffuse,specular
  float ns,ni,d;//specular exponent,refraction,transparency
  int illum;//illumination model
  mtl(string&);
};

struct obj_desc {
  vector<vec3> vs,vts,ns;
  vector<face> fs;
  vector<mtl> mtls;
  obj_desc(string&);
  vec3 f2v(long,int);
};
