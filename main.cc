#include<algorithm>
#include<iostream>
#include<fstream>
#include"kdtree.hh"
#include"obj.hh"

constexpr int WIDTH=1000,HEIGHT=1000;
constexpr double r=double(WIDTH)/double(HEIGHT);

void r2s(double i,double j,double&x,double&y,double fov){
  double t=tan(radians(fov*0.5));
  x=(2*((i+0.5)/double(WIDTH))-1)*t*r;
  y=(1-2*((j+0.5)/double(HEIGHT)))*t;
}
int main(int argc,char*argv[]){
  if(argc<3) return 1;
  string in(argv[1]);
  ofstream out(argv[2]);
  out<<"P3"<<endl<<WIDTH<<' '<<HEIGHT<<endl<<255<<endl;
  aabb b; dvec3 v;
  vector<light> ls;
  ls.push_back({.p={10,10,10},.c={0,0,0},.ia=.9,.id=.3,.is=.3});
  //ray r({5,5,-5});
  ray r({8,8,8});
  obj*o=new obj(in);
  double x,y;
  if(GOOD==o->st){
    vector<long> ts(o->fs.size());
    for(size_t i=0; i<ts.size(); ++i) ts[i]=i;
    for(auto v:o->vs)
      for(int i=0; i<3; ++i){
        b.f[i]=std::min(b.f[i],v[i]);
        b.t[i]=std::max(b.t[i],v[i]);
      }
    kdtree*t=new kdtree(o,b,0,ts);
    for(int i=0; i<WIDTH; ++i){
      for(int j=0; j<HEIGHT; ++j){
        dvec3 I(0,0,0);
        r2s(double(i),double(j),x,y,90.0);
        r.direct(x,y);
    	t->hit(o,r,I,v,ls,0); saturate(I); I*=255;
    	out<<int(I.x)<<' '<<int(I.y)<<' '<<int(I.z)<<' ';
      }
      out<<endl;
    }
    delete t;
  }
  delete o;
  out.close();
  return 0;
}
