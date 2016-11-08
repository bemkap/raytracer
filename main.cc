#include<algorithm>
#include<fstream>
#include"kdtree.hh"
#include"obj.hh"
#include"sdl.hh"

#include<iostream>
using namespace std;

int main(int argc,char*argv[]){
  if(argc<3) return 1;
  string in(argv[1]);
  ofstream os(argv[2],ofstream::out);
  os<<"P3"<<endl<<WIDTH<<' '<<HEIGHT<<endl<<255<<endl;
  dvec3 v,n; aabb b;
  vector<light> ls;
  ray r({0,-3,30},60.0);
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
    ls.push_back({{0,-3,30},{255,255,255},1,1,1});
    double x,y,ir=double(WIDTH)/double(HEIGHT);
    for(int i=0; i<WIDTH; ++i){
      for(int j=0; j<HEIGHT; ++j){
        dvec3 I(0,0,0);
        sdl::r2s(double(i),double(j),x,y,r.fov,ir);
        r.direct(x,y);
        if(t->hit(o,r,v,n)) I=255.0*(o->mtls.size()>0?o->mtls[0]->I(ls,v,n,r.o):dvec3(1,1,1));
        os<<int(I.x)<<' '<<int(I.y)<<' '<<int(I.z)<<' ';
      }
      os<<endl;
    }
    delete t;
  }
  os.close();
  delete o;
  return 0;
}
