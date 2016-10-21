#include"vec3.h"

vec3 vec3_add(vec3 u,vec3 v){
  return (vec3){u.x+v.x,u.y+v.y,u.z+v.z};
}
vec3 vec3_sub(vec3 u,vec3 v){
  return (vec3){u.x-v.x,u.y-v.y,u.z-v.z};
}
vec3 vec3_mul(float u,vec3 v){
  return (vec3){u*v.x,u*v.y,u*v.z};
}
vec3 vec3_div(vec3 u,vec3 v){
  return (vec3){u.x/v.x,u.y/v.y,u.z/v.z};
}
vec3 vec3_cross(vec3 u,vec3 v){
  return (vec3){u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x};
}
float vec3_dot(vec3 u,vec3 v){
  return u.x*v.x+u.y*v.y+u.z*v.z;
}
