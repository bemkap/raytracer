#include<algorithm>
#include<functional>
#include<iostream>
#include<fstream>
#include<regex>
#include<random>
#include"kdtree.hh"
#include"obj.hh"

constexpr int WIDTH=480,HEIGHT=640;
constexpr double RATIO=double(WIDTH)/double(HEIGHT);
constexpr double SAMPLES=1;

void r2s(double i,double j,double&x,double&y,double fov){
  double t=tan(radians(fov*0.5));
  x=(2*(i/double(WIDTH))-1)*t*RATIO;
  y=(1-2*(j/double(HEIGHT)))*t;
}
int main(int argc,char*argv[]){
  if(argc<3) return 1;
  string in(argv[1]);
  ofstream out(argv[2]);
  aabb b; dvec3 v;
  vector<light> ls;
  default_random_engine gen;
  uniform_real_distribution<double> dist(0.0,1.0);
  auto rand=bind(dist,gen);
  ray r({1,1,2});
  ls.push_back({.p=r.o,.c={0,0,0},.ia=.9,.id=.9,.is=.9});
  obj*o=new obj(in);
  double x,y;
  if(GOOD==o->st){
    vector<size_t> ts(o->fs.size());
    for(size_t i=0; i<ts.size(); ++i) ts[i]=i;
    for(auto v:o->vs)
      for(int i=0; i<3; ++i){
        b.f[i]=std::min(b.f[i],v[i]);
        b.t[i]=std::max(b.t[i],v[i]);
      }
    kdtree*t=new kdtree(o,b,0,ts);
    out<<"P3"<<endl<<HEIGHT<<' '<<WIDTH<<endl<<255<<endl;
    for(int i=0; i<WIDTH; ++i){
      for(int j=0; j<HEIGHT; ++j){
        dvec3 I(0,0,0);
  	for(int k=0; k<SAMPLES; ++k){
  	  r2s(double(i)+rand(),double(j)+rand(),x,y,90.0);
  	  r.direct(x,y);
  	  t->hit(o,r,I,v,ls,0);
  	}
  	I*=1.0/SAMPLES; //saturate(I); I*=255;
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
