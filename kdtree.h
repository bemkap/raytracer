#pragma once

typedef union { float p[3]; struct { float x,y,z; }; } vec3;
typedef union { vec3  v[3]; struct { vec3  p,q,r; }; } tri;

struct kdtree {
  tri node;
  unsigned depth;
  struct kdtree*left,*right;
};

struct kdtree*kdtree_new(tri*,unsigned);
struct kdtree*kdtree_leaf(tri,unsigned);
struct kdtree*kdtree_add(struct kdtree*,tri);
struct kdtree*kdtree_remove(struct kdtree*,tri);
struct kdtree*kdtree_nearest(struct kdtree*,tri);
unsigned kdtree_count(struct kdtree*);
void     kdtree_destroy(struct kdtree*);
