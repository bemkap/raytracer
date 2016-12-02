#pragma once

#include<fstream>
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
  ofstream out;
  setup(const string&,int,int); ~setup();
};
