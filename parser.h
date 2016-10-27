#pragma once

#include<glib.h>

enum { INITIAL_SIZE = 1024 };

typedef union {
  int a[9];
  struct { int v0,t0,n0, v1,t1,n1, v2,t2,n2; };
} face;

typedef struct {
  GPtrArray*vs,*vts,*ns,*fs;
} obj_desc;

obj_desc*obj_parse(const char*);
void     obj_destroy(obj_desc*);
