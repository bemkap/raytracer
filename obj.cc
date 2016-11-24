#include<cstdio>
#include<fstream>
#include<iostream>
#include"obj.hh"
using namespace std;

size_t face::operator[](int i){return a[i%9];}
obj::obj(string&fn){
  string line; char str[128];
  ifstream in(fn); face f; dvec3 v;
  mat*cm;
  if(in.rdstate()&ifstream::failbit) st=BAD;
  else{
    while(getline(in,line)){
      if(3==sscanf(line.c_str(),"f %d %d %d",&f.v0,&f.v1,&f.v2)){
        f.t0=f.t1=f.t2=f.n0=f.n1=f.n2=-1; f.m=cm;
        fs.push_back(f);
      }else if(6==sscanf(line.c_str(),"f %d//%d %d//%d %d//%d",&f.v0,&f.n0, &f.v1,&f.n1, &f.v2,&f.n2)){
	f.t0=f.t1=f.t2=-1; f.m=cm;
	fs.push_back(f);
      }else if(9==sscanf(line.c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&f.v0,&f.t0,&f.n0, &f.v1,&f.t1,&f.n1, &f.v2,&f.t2,&f.n2)){
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
double obj::get_vert(size_t f,VERT v,AXIS a){
  return vs[fs[f][v]-1][a];
}
dvec3 obj::get_vert(size_t f,VERT v){
  return vs[fs[f][v]-1];
}
double obj::get_norm(size_t f,NORM n,AXIS a){
  return ns[fs[f][n]-1][a];
}
dvec3 obj::get_norm(size_t f,NORM n){
  return ns[fs[f][n]-1];
}
double obj::min3(size_t f,AXIS a){
  return std::min(get_vert(f,V0,a),
		  std::min(get_vert(f,V1,a),
			   get_vert(f,V2,a)));
}
double obj::max3(size_t f,AXIS a){
  return std::max(get_vert(f,V0,a),
		  std::max(get_vert(f,V1,a),
			   get_vert(f,V2,a)));
}
triangle obj::get_tri(size_t f){
  return {get_vert(f,V0),get_vert(f,V1),get_vert(f,V2)};
}
void obj::stats(){
  cout<<vs.size()<<" vertices"<<endl;
  cout<<ns.size()<<" normals"<<endl;
  cout<<fs.size()<<" faces"<<endl;
}
