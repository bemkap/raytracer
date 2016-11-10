#include<iostream>
#include<algorithm>
#include<fstream>
#include"kdtree.hh"
#include"obj.hh"

constexpr int WIDTH=500,HEIGHT=500;

void r2s(double i,double j,double&x,double&y,double fov,double r){
  x=(2*((i+0.5)/double(WIDTH))-1)*tan(radians(fov/2))*r;
  y=(1-2*((j+0.5)/double(HEIGHT)))*tan(radians(fov/2));
}
int main(int argc,char*argv[]){
  if(argc<2) return 1;
  string in(argv[1]);
  cout<<"P3"<<endl<<WIDTH<<' '<<HEIGHT<<endl<<255<<endl;
  aabb b;
  vector<light> ls;
  ray r({0,0,3});
  obj*o=new obj(in);
  if(GOOD==o->st){
    vector<long> ts(o->fs.size());
    generate_n(ts.begin(),o->fs.size(),[](){static long n=0; return n++;});
    for(auto v:o->vs)
      for(int i=0; i<3; ++i){
        b.f[i]=std::min(b.f[i],v[i]);
        b.t[i]=std::max(b.t[i],v[i]);
      }
    kdtree*t=new kdtree(o,b,0,ts);
    ls.push_back({{0,-3,30},{0,0,0},1,1,1});
    double x,y,ir=double(WIDTH)/double(HEIGHT);
    for(int i=0; i<WIDTH; ++i){
      for(int j=0; j<HEIGHT; ++j){
        dvec3 I(0,0,0);
        r2s(double(i),double(j),x,y,60.0,ir);
        r.direct(x,y);
        t->hit(o,r,I,ls,0);
        cout<<int(255.0*I.x)<<' '<<int(255.0*I.y)<<' '<<int(255.0*I.z)<<' ';
      }
      cout<<endl;
    }
    delete t;
  }
  delete o;
  return 0;
}
