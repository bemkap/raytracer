#include<algorithm>
#include<functional>
#include<iostream>
#include<fstream>
#include<regex>
#include<random>
#include"kdtree.hh"
#include"obj.hh"

constexpr int WIDTH=640,HEIGHT=480;
constexpr double RATIO=double(WIDTH)/double(HEIGHT);
constexpr double SAMPLES=1,FOV=90.0;

int main(int argc,char*argv[]){
  if(argc<3) return 1;
  string in(argv[1]);
  ofstream out(argv[2]);
  aabb b; dvec3 v;
  vector<light> ls;
  default_random_engine gen;
  uniform_real_distribution<double> dist(0.0,1.0);
  auto rand=bind(dist,gen);
  ray r({0.5,0.5,0.5});
  ls.push_back({.p=r.o,.c={1,1,1},.ia=20,.id=20,.is=20});
  obj*o=new obj(in);
  double x,y,z=tan(radians(FOV*0.5));
  if(GOOD==o->st){
    vector<size_t> ts(o->fs.size());
    for(size_t i=0; i<ts.size(); ++i) ts[i]=i;
    for(auto v:o->vs)
      for(int i=0; i<3; ++i){
        b.f[i]=std::min(b.f[i],v[i]);
        b.t[i]=std::max(b.t[i],v[i]);
      }
    kdtree*t=new kdtree(o,b,0,ts);
    out<<"P3"<<endl<<WIDTH<<' '<<HEIGHT<<endl<<255<<endl;
    for(int j=0; j<HEIGHT; ++j){
      for(int i=0; i<WIDTH; ++i){
        dvec3 I(0,0,0);
  	for(int k=0; k<SAMPLES; ++k){
	  x=(2*((double(i)+rand())/double(WIDTH))-1)*z*RATIO;
	  y=(1-2*((double(j)+rand())/double(HEIGHT)))*z;
  	  r.direct(x,y);
  	  t->hit(o,r,I,v,ls,0);
  	}
  	I*=1.0/SAMPLES; saturate(I);
  	out<<int(I.x)<<' '<<int(I.y)<<' '<<int(I.z)<<' ';
      }
      out<<endl;
    }
    delete t;
  }else cout<<"NOT GOOD"<<endl;
  delete o;
  out.close();
  return 0;
}
