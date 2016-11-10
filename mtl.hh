#pragma once

#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
#include<map>
#include<string>
#include"prim.hh"
using namespace glm;
using namespace std;

enum state {GOOD,BAD};

class mtl {public:
  state st;
  map<string,mat> ms;
  mtl(string&); ~mtl();
};

class mat {public:
  dvec3 ka,kd,ks;//ambient,diffuse,specular
  double ns,ni,d;//specular exponent,refraction,transparency
  int illum;//illumination model
  dvec3 I(vector<light>&,dvec3,dvec3,dvec3);
};
