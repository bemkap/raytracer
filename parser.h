#pragma once

enum { INITIAL_SIZE = 1024 };

struct container {
  void**ar;
  unsigned long av,sz;
};

typedef union {
  int a[9];
  struct { int v0,t0,n0, v1,t1,n1, v2,t2,n2; };
} face;

struct obj_desc {
  struct container*vs,*vts,*ns,*fs;
};

struct   container*container_new(unsigned);
unsigned container_push(struct container*,void*);
void     container_destroy(struct container*);

struct obj_desc*obj_parse(const char*);
void   obj_destroy(struct obj_desc*);
