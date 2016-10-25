#pragma once

#include<glib.h>
#include"vec3.h"
#include"parser.h"

typedef union { vec3 v[3]; struct { vec3 p,q,r; }; } tri;

struct box { vec3 from,to; };
struct ray { vec3 p0,dir; };

struct kdtree {
  tri node;
  vec3 n,vt;
  unsigned depth;
  struct box bounds;
  struct kdtree*left,*right;
};

struct kdtree*kdtree_new(struct obj_desc*,unsigned);
struct kdtree*kdtree_leaf(tri,unsigned);
struct kdtree*kdtree_add(struct kdtree*,tri);
struct kdtree*kdtree_nearest(struct kdtree*,tri);
int kdtree_hit(struct kdtree*,struct ray,vec3*);
unsigned kdtree_count(struct kdtree*);
void kdtree_destroy(struct kdtree*);

int box_hit(struct box,struct ray);
