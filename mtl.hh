#pragma once

#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
#include<string>
#include<vector>
#include"prim.hh"
using namespace glm;
using namespace std;

enum state {GOOD,BAD};

class mtl {public:
  state st;
  string name;
  dvec3 ka,kd,ks;//ambient,diffuse,specular
  double ns,ni,d;//specular exponent,refraction,transparency
  int illum;//illumination model
  mtl(string&);
  dvec3 I(vector<light>&,dvec3,dvec3,dvec3);
};
