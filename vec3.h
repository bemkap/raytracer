#pragma once

typedef union { float p[3]; struct { float x,y,z; }; } vec3;

vec3 vec3_add(vec3,vec3);
vec3 vec3_sub(vec3,vec3);
vec3 vec3_mul(float,vec3);
vec3 vec3_div(vec3,vec3);
vec3 vec3_cross(vec3,vec3);
float vec3_dot(vec3,vec3);
