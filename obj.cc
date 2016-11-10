#include<cstdio>
#include<fstream>
#include<map>
#include"obj.hh"
using namespace std;

long face::operator[](int i){
  return (long[]){v0,t0,n0,v1,t1,n1,v2,t2,n2}[i%9];
}
obj::obj(string&fn){
  string line; char str[128];
  ifstream in(fn); face f; dvec3 v;
  map<string,char> mm; char cm=-1;
  if(in.rdstate()&ifstream::failbit) st=BAD;
  else{
    while(getline(in,line)){
      if(3==sscanf(line.c_str(),"f %ld %ld %ld",&f.v0,&f.v1,&f.v2)){
        f.t0=f.t1=f.t2=f.n0=f.n1=f.n2=-1; f.m=cm;
        fs.push_back(f);
      }else if(6==sscanf(line.c_str(),"f %ld//%ld %ld//%ld %ld//%ld",&f.v0,&f.n0, &f.v1,&f.n1, &f.v2,&f.n2)){
        f.t0=f.t1=f.t2=-1; f.m=cm;
        fs.push_back(f);
      }else if(9==sscanf(line.c_str(),"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld",&f.v0,&f.t0,&f.n0, &f.v1,&f.t1,&f.n1, &f.v2,&f.t2,&f.n2)){
        f.m=cm;
        fs.push_back(f);
      }else if(3==sscanf(line.c_str(),"vn %lf %lf %lf",&v.x,&v.y,&v.z))
        ns.push_back(v);
      else if(2==sscanf(line.c_str(),"vt %lf %lf",&v.x,&v.y))
        vts.push_back(v);
      else if(3==sscanf(line.c_str(),"v %lf %lf %lf",&v.x,&v.y,&v.z))
        vs.push_back(v);
      else if(1==sscanf(line.c_str(),"mtllib %s",str)){
        string fn(str);
        mtl*m=new mtl(fn);
        if(GOOD==m->st){mm[m->name]=mtls.size(); mtls.push_back(m);}
        else delete m;
      }else if(1==sscanf(line.c_str(),"usemtl %s",str))
        cm=mm.at(str);
    }
    st=GOOD;
  }
  in.close();
}
obj::~obj(){for(auto m:mtls) delete m;}
dvec3 obj::f2v(long i,int v){return vs[fs[i][3*(v%3)]-1];}
dvec3 obj::f2n(long i,int v){
  if(-1==fs[i][2+3*(v%3)]) throw -1;
  return ns[fs[i][2+3*(v%3)]-1];}
triangle obj::f2t(long i){return {f2v(i,0),f2v(i,1),f2v(i,2)};}
double obj::min3(long i,unsigned d){
  return std::min(f2v(i,0)[d%3],std::min(f2v(i,1)[d%3],f2v(i,2)[d%3]));
}
double obj::max3(long i,unsigned d){
  return std::max(f2v(i,0)[d%3],std::max(f2v(i,1)[d%3],f2v(i,2)[d%3]));
}
