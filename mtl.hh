#pragma once

#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
#include<map>
#include<string>
#include<vector>
#include"prim.hh"
using namespace glm;
using namespace std;

enum state {GOOD,BAD};

class mat {public:
  dvec3 ka,kd,ks;//ambient,diffuse,specular
  double ns,ni,d;//specular exponent,refraction,transparency
  double ir,it;//intensity reflected and transmitted direction
  int illum;//illumination model
  dvec3 I(vector<light>&,dvec3&,dvec3&,dvec3&,double);
};

namespace mtl {
  bool read(string&,map<string,mat*>&);
};
