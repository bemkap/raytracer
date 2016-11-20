#include<cstdio>
#include<fstream>
#include<map>
#include"obj.hh"
using namespace std;

long face::operator[](int i){return a[i%9];}
obj::obj(string&fn){
  string line; char str[128];
  ifstream in(fn); face f; dvec3 v;
  mat*cm;
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
      	mtl::read(fn,mm);
      }else if(1==sscanf(line.c_str(),"usemtl %s",str))
        cm=mm.at(str);
    }
    st=GOOD;
  }
  in.close();
}
obj::~obj(){for(auto m:mm) delete m.second;}
