#pragma once

#include<pthread.h>
#include<vector>
#include"kdtree.hh"
#include"obj.hh"
#include"prim.hh"

class setup {public:
  unsigned char*scr;
  ray r;
  kdtree*t;
  obj*o;
  vector<light> ls;
  setup(const string&,int,int); ~setup();
};
