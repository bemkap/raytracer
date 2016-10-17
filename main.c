#include"kdtree.h"

void main(){
  vec3 ns[]={{2,3,8},{5,4,3},{9,6,1},{4,7,7},{8,1,9},{7,2,3}};
  struct kdtree*t=kdtree_new(ns,6);
  kdtree_print(t);
  kdtree_destroy(t);
}
