#pragma once

#include"vec3.h"

typedef union { vec3  v[3]; struct { vec3  p,q,r; }; } tri;

struct box { vec3 from,to; };
struct ray { vec3 p0,dir;  };

struct kdtree {
  tri node;
  unsigned depth;
  struct box bounds;
  struct kdtree*left,*right;
};

struct kdtree*kdtree_new(tri*,unsigned);
struct kdtree*kdtree_leaf(tri,unsigned);
struct kdtree*kdtree_add(struct kdtree*,tri);
struct kdtree*kdtree_remove(struct kdtree*,tri);
struct kdtree*kdtree_nearest(struct kdtree*,tri);
int kdtree_hit(struct kdtree*,struct ray,vec3*);
unsigned kdtree_count(struct kdtree*);
void kdtree_destroy(struct kdtree*);
