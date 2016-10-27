#pragma once

#include<glib.h>
#include"vec3.h"
#include"parser.h"

typedef union { vec3 v[3]; struct { vec3 p,q,r; }; } tri;

typedef struct { vec3 f,t; } box;
typedef struct { vec3 p,d; } ray;

typedef struct _kdtree {
  long i,j;
  vec3 n,vt;
  unsigned depth;
  box bounds;
  struct _kdtree*left,*right;
} kdtree;

kdtree*kdtree_new(obj_desc*,box,unsigned);
kdtree*kdtree_leaf(long,long,unsigned);
int    kdtree_hit(obj_desc*,kdtree*,ray,vec3*);
void   kdtree_destroy(kdtree*);

int box_hit(box,ray);
