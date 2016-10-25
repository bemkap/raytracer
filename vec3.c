#include<stdio.h>
#include"vec3.h"

vec3 vec3_add(vec3 u,vec3 v){ vec3 w; w.x=u.x+v.x; w.y=u.y+v.y; w.z=u.z+v.z; return w; }
vec3 vec3_sub(vec3 u,vec3 v){ vec3 w; w.x=u.x-v.x; w.y=u.y-v.y; w.z=u.z-v.z; return w; }
vec3 vec3_mul(float u,vec3 v){ vec3 w; w.x=u*v.x; w.y=u*v.y; w.z=u*v.z; return w; }
vec3 vec3_div(vec3 u,vec3 v){ vec3 w; w.x=u.x/v.x; w.y=u.y/v.y; w.z=u.z/v.z; return w; }
vec3 vec3_cross(vec3 u,vec3 v){ vec3 w; w.x=u.y*v.z-u.z*v.y; w.y=u.z*v.x-u.x*v.z; w.z=u.x*v.y-u.y*v.x; return w; }
float vec3_dot(vec3 u,vec3 v){ return u.x*v.x+u.y*v.y+u.z*v.z; }
void vec3_print(vec3 u){ printf("(%f,%f,%f) ",u.x,u.y,u.z); }
