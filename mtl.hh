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
  vec3 ka,kd,ks;//ambient,diffuse,specular
  float ns,ni,d;//specular exponent,refraction,transparency
  int illum;//illumination model
  mtl(string&);
  vec3 I(vector<light>&,vec3,vec3,vec3);
};
