#include<cstdio>
#include<fstream>
#include<string.h>
#include<glm/vec3.hpp>
#include"parser.hh"
using namespace std;

long face::operator[](int i){return a[i%9];}
mtl::mtl(string&fn){
  string line; char str[128]={0};
  ifstream in(fn);
  while(getline(in,line)){
    if(sscanf(line.c_str(),"newmtl %s",str)) name=string(str,strlen(str));
    else if(sscanf(line.c_str(),"Ns %f",&ns));
    else if(sscanf(line.c_str(),"Ka %f %f %f",&ka.x,&ka.y,&ka.z));
    else if(sscanf(line.c_str(),"Kd %f %f %f",&kd.x,&kd.y,&kd.z));
    else if(sscanf(line.c_str(),"Ks %f %f %f",&ks.x,&ks.y,&ks.z));
    else if(sscanf(line.c_str(),"Ni %f",&ni));
    else if(sscanf(line.c_str(),"d %f",&d));
    else if(sscanf(line.c_str(),"illum %d",&illum));
  }
  in.close();
}
obj_desc::obj_desc(string&fn){
  string line; char str[128];
  ifstream in(fn); face f; vec3 v;
  while(getline(in,line)){
    if(sscanf(line.c_str(),"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld",
              &f.v0,&f.t0,&f.n0, &f.v1,&f.t1,&f.n1, &f.v2,&f.t2,&f.n2))
      fs.push_back(f);
    else if(sscanf(line.c_str(),"vn %f %f %f",&v.x,&v.y,&v.z))
      ns.push_back(v);
    else if(sscanf(line.c_str(),"vt %f %f",&v.x,&v.y))
      vts.push_back(v);
    else if(sscanf(line.c_str(),"v %f %f %f",&v.x,&v.y,&v.z))
      vs.push_back(v);
    else if(sscanf(line.c_str(),"mtllib %s",str)){
      string fn(str);
      mtls.push_back(mtl(fn));
    }
  }
  in.close();
}
vec3 obj_desc::f2v(long i,int v){return vs[fs[i][3*(v%3)]-1];}
vec3 obj_desc::f2n(long i,int v){return ns[fs[i][2+3*(v%3)]-1];}
triangle obj_desc::f2t(long i){return {f2v(i,0),f2v(i,1),f2v(i,2)};}
