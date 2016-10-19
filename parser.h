#pragma once

#include"kdtree.h"

enum { INITIAL_SIZE = 1024 };

struct container {
  void**ar;
  unsigned av,sz;
};

struct obj_desc {
  struct container*vs,*vts,*ns,*fs;
};

struct   container*container_new(unsigned);
unsigned container_push(struct container*,void*);
void     container_destroy(struct container*);

struct obj_desc*obj_parse(const char*);
void   obj_destroy(struct obj_desc*);
