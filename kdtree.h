#pragma once

typedef union { float p[3]; struct { float x,y,z; }; } vec3;

struct kdtree {
  vec3 node;
  unsigned depth;
  struct kdtree*left,*right;
};

struct kdtree*kdtree_new(vec3*,unsigned);
struct kdtree*kdtree_leaf(vec3,unsigned);
struct kdtree*kdtree_add(struct kdtree*,vec3);
struct kdtree*kdtree_remove(struct kdtree*,vec3);
struct kdtree*kdtree_nearest(struct kdtree*,vec3);
unsigned kdtree_count(struct kdtree*);
void   kdtree_destroy(struct kdtree*);
void   kdtree_print(struct kdtree*);
