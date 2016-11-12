#include<iostream>
#include<fstream>
#include"kdtree.hh"
#include"obj.hh"

constexpr int WIDTH=500,HEIGHT=500;
constexpr double r=double(WIDTH)/double(HEIGHT);

void r2s(double i,double j,double&x,double&y,double fov){
  double t=tan(radians(fov*0.5));
  x=(2*((i)/double(WIDTH))-1)*t*r;
  y=(1-2*((j)/double(HEIGHT)))*t;
}
int main(int argc,char*argv[]){
  if(argc<2) return 1;
  string in(argv[1]);
  cout<<"P3"<<endl<<WIDTH<<' '<<HEIGHT<<endl<<255<<endl;
  aabb b;
  vector<light> ls;
  ls.push_back({{0,10,10},{0,0,0},10,10,10});
  ray r({50,50,50});
  obj*o=new obj(in);
  double x,y;
  if(GOOD==o->st){
    vector<long> ts(o->fs.size());
    for(unsigned long i=0; i<ts.size(); ++i) ts[i]=i;
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
