#pragma once

#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include<glm/vec3.hpp>
using namespace std;
using namespace glm;

enum AXIS {X,Y,Z};
enum VERT {V0=0,V1=3,V2=6};
enum TEXT {T0=1,T1=4,T2=7};
enum NORM {N0=2,N1=5,N2=8};

struct aabb     {dvec3 f,t;};
struct plane    {double e; AXIS k;};
struct triangle {dvec3 p,q,r;};
struct light    {dvec3 p,c; double ia,id,is;};
