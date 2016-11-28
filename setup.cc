#include"setup.hh"

setup::setup(const string&o_fn,int width,int height){
  o=new obj(o_fn);
  if(o->st==GOOD){
    aabb b;
    r=ray({0,0,-0.8});
    ls.push_back({.p=r.o,.c={1,1,1},.ia=1,.id=1,.is=1});
    vector<size_t> ts(o->fs.size());
    for(size_t i=0; i<ts.size(); ++i) ts[i]=i;
    for(auto v:o->vs)
      for(int i=0; i<3; ++i){
	b.f[i]=std::min(b.f[i],v[i]);
	b.t[i]=std::max(b.t[i],v[i]);
      }
    t=new kdtree(o,b,0,ts);
    scr=new unsigned char[height*width*3];
  }else throw;
}
setup::~setup(){
  delete t;
  delete o;
  delete[] scr;
}
