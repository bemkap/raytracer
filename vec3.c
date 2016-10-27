#include<stdio.h>
#include<math.h>
#include"vec3.h"

vec3  vec3_add(vec3 u,vec3 v){return (vec3){{u.x+v.x,u.y+v.y,u.z+v.z}};}
vec3  vec3_sub(vec3 u,vec3 v){return (vec3){{u.x-v.x,u.y-v.y,u.z-v.z}};}
vec3  vec3_mul(float u,vec3 v){return (vec3){{u*v.x,u*v.y,u*v.z}};}
float vec3_mag(vec3 u){return sqrt(vec3_dot(u,u));}
vec3  vec3_normal(vec3 u){float d=1/vec3_mag(u); return vec3_mul(d,u);}
vec3  vec3_cross(vec3 u,vec3 v){return (vec3){{u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x}};}
float vec3_dot(vec3 u,vec3 v){return u.x*v.x+u.y*v.y+u.z*v.z;}
void  vec3_print(vec3 u){printf("(%f,%f,%f) ",u.x,u.y,u.z);}
