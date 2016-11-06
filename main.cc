#include"main.hh"
using namespace cfg;

#include<iostream>
using namespace std;

int main(int argc,char*argv[]){
  if(argc<2) return 1;
  string fn(argv[1]);
  vec3 v,n; aabb b;
  obj*o=new obj(fn);
  if(GOOD==o->st){
    sdl*g=new sdl();
    vector<long> ts(o->fs.size());
    generate_n(ts.begin(),o->fs.size(),[](){ static long n=0; return n++; });
    for(auto v:o->vs)
      for(int i=0; i<3; ++i){
        b.f[i]=std::min(b.f[i],v[i]);
        b.t[i]=std::max(b.t[i],v[i]);
      }
    kdtree*t=new kdtree(o,b,0,ts);
    ls.push_back({{10,16,-30},{255,255,255},1,1,1});
    for(int i=0; i<WIDTH; ++i)
      for(int j=0; j<HEIGHT; ++j){
        r.direct(-1.0+i*(2.0/WIDTH),-1.0+j*(2.0/HEIGHT));
        if(t->hit(o,r,v,n)){
          vec3 I=o->mtls[0]->I(ls,v,n,r.o);
          g->set(j,i,I);
        }
      }
    g->draw();
    g->waitexit();
    delete t;
    delete g;
  }
  delete o;
  return 0;
}
