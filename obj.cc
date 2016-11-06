#include<cstdio>
#include<fstream>
#include"obj.hh"
using namespace std;

long face::operator[](int i){return a[i%9];}
obj::obj(string&fn){
  string line; char str[128];
  ifstream in(fn); face f; vec3 v;
  if(in.rdstate()&ifstream::failbit) st=BAD;
  else{
    while(getline(in,line)){
      if(6==sscanf(line.c_str(),"f %ld//%ld %ld//%ld %ld//%ld",
                   &f.v0,&f.n0, &f.v1,&f.n1, &f.v2,&f.n2)){
        f.t0=f.t1=f.t2=-1;
        fs.push_back(f);
      }else if(9==sscanf(line.c_str(),"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld",
                         &f.v0,&f.t0,&f.n0, &f.v1,&f.t1,&f.n1, &f.v2,&f.t2,&f.n2))
        fs.push_back(f);
      else if(3==sscanf(line.c_str(),"vn %f %f %f",&v.x,&v.y,&v.z))
        ns.push_back(v);
      else if(2==sscanf(line.c_str(),"vt %f %f",&v.x,&v.y))
        vts.push_back(v);
      else if(3==sscanf(line.c_str(),"v %f %f %f",&v.x,&v.y,&v.z))
        vs.push_back(v);
      else if(1==sscanf(line.c_str(),"mtllib %s",str)){
        string fn(str);
        mtl*m=new mtl(fn);
        if(GOOD==m->st) mtls.push_back(m);
        else delete m;
      }
    }
    in.close();
    st=GOOD;
  }
}
obj::~obj(){for(auto m:mtls) delete m;}
vec3 obj::f2v(long i,int v){return vs[fs[i][3*(v%3)]-1];}
vec3 obj::f2n(long i,int v){return ns[fs[i][2+3*(v%3)]-1];}
triangle obj::f2t(long i){return {f2v(i,0),f2v(i,1),f2v(i,2)};}
float obj::min3(long i,unsigned d){
  return std::min(f2v(i,0)[d%3],std::min(f2v(i,1)[d%3],f2v(i,2)[d%3]));
}
float obj::max3(long i,unsigned d){
  return std::max(f2v(i,0)[d%3],std::max(f2v(i,1)[d%3],f2v(i,2)[d%3]));
}
