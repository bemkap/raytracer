#include<algorithm>
#include"kdtree.hh"
#include"obj.hh"
#include"sdl.hh"

#include<iostream>
using namespace std;

int main(int argc,char*argv[]){
  if(argc<2) return 1;
  string fn(argv[1]);
  dvec3 v,n; aabb b;
  vector<light> ls;
  ray r({0,3,30},60.0,{0,0,0});
  obj*o=new obj(fn);
  if(GOOD==o->st){
    sdl*g=new sdl();
    vector<long> ts(o->fs.size());
    generate_n(ts.begin(),o->fs.size(),[](){static long n=0; return n++;});
    for(auto v:o->vs)
      for(int i=0; i<3; ++i){
        b.f[i]=std::min(b.f[i],v[i]);
        b.t[i]=std::max(b.t[i],v[i]);
      }
    kdtree*t=new kdtree(o,b,0,ts);
    ls.push_back({{0,0,3.5},{255,255,255},1,1,1});
    bool quit=false, redraw=true;
    while(!quit){if(redraw){
        double x,y;
        g->clear();
        for(int i=0; i<WIDTH; ++i){
          for(int j=0; j<HEIGHT; ++j){
            g->r2s(double(i),double(j),x,y,r.fov);
            r.direct(x,y);
            // r.direct(-1+i*(2.0/WIDTH),-1+j*(2.0/HEIGHT));
            if(t->hit(o,r,v,n)){
              dvec3 I=o->mtls.size()>0?o->mtls[0]->I(ls,v,n,r.o):dvec3(1,1,1);
              g->set(j,i,I);
            }
          }
        }
        g->draw();
        SDL_Delay(1000);
      }
      g->wait_input(r.o,quit,redraw);
      r=ray(r.o,60.0,{0,0,0});
    }
    delete t;
    delete g;
  }
  delete o;
  return 0;
}
