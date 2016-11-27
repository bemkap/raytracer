#include<fstream>
#include<string>
#include"obj.hh"
using namespace std;

typedef void(*handle)(obj*,const smatch&);
void h_v(obj*o,const smatch&cm){o->vs.push_back(dvec3(stod(cm[1]),stod(cm[2]),stod(cm[3])));}
void h_vt(obj*o,const smatch&cm){o->has_vts=true; o->vts.push_back(dvec3(stod(cm[1]),stod(cm[2]),0));}
void h_vn(obj*o,const smatch&cm){o->has_ns=true; o->ns.push_back(dvec3(stod(cm[1]),stod(cm[2]),stod(cm[3])));}
void h_f3(obj*o,const smatch&cm){face f; f.m=o->cmtl;
  for(int i=1; i<10; ++i) f.a[i-1]=0==cm[i].length()?-1:stoi(cm[i]);
  o->fs.push_back(f);
}
void h_f4(obj*o,const smatch&cm){face f; f.m=o->cmtl;
  for(int i=0; i<9; ++i) f.a[i]=0==cm[i+1].length()?-1:stoi(cm[i+1]);
  o->fs.push_back(f);
  for(int i=6; i<15; ++i) f.a[i-6]=0==cm[i%12+1].length()?-1:stoi(cm[i%12+1]);
  o->fs.push_back(f);
}
void h_mtllib(obj*o,const smatch&cm){string s(cm[1]); mtl::read(s,o->mm);}
void h_usemtl(obj*o,const smatch&cm){o->cmtl=o->mm.at(cm[1]);}
void h_nop(obj*o,const smatch&cm){}

size_t face::operator[](int i){return a[i%9];}
obj::obj(string&fn):has_vts(false),has_ns(false){
  string line;
  ifstream in(fn);
  string r_double="((?:\\+|-)?\\d+(?:\\.\\d+)?(?:[eE]-?\\d+)?)";
  string r_vec3=r_double+"\\s+"+r_double+"\\s+"+r_double;
  string r_vec2=r_double+"\\s+"+r_double;
  regex r_obj[]={
    regex("v\\s"+r_vec3), regex("vt\\s+"+r_vec2), regex("vn\\s+"+r_vec3),
    regex("f\\s+(\\d+)(?:/(\\d+)?)?(?:/(\\d+))?\\s+(\\d+)(?:/(\\d+)?)?(?:/(\\d+))?"
	  "\\s+(\\d+)(?:/(\\d+)?)?(?:/(\\d+))?\\s+(\\d+)(?:/(\\d+)?)?(?:/(\\d+))?"),
    regex("f\\s+(\\d+)(?:/(\\d+)?)?(?:/(\\d+))?\\s+(\\d+)(?:/(\\d+)?)?(?:/(\\d+))?"
	  "\\s+(\\d+)(?:/(\\d+)?)?(?:/(\\d+))?"),
    regex("mtllib\\s+(.+)"), regex("usemtl\\s+(.+)")
  };
  handle handles[]={h_v,h_vt,h_vn,h_f4,h_f3,h_mtllib,h_usemtl,h_nop};
  smatch match;
  if(in.rdstate()&ifstream::failbit) st=BAD;
  else{
    while(getline(in,line)){
      int i; for(i=0; i<7&&!regex_match(line,match,r_obj[i]); ++i);
      handles[i](this,match);
    }
    st=GOOD;
  }
  in.close();
}
obj::~obj(){for(auto m:mm) delete m.second;}
double obj::get_vert(size_t f,VERT v,AXIS a){return vs[fs[f][v]-1][a];}
dvec3 obj::get_vert(size_t f,VERT v){return vs[fs[f][v]-1];}
double obj::get_norm(size_t f,NORM n,AXIS a){return ns[fs[f][n]-1][a];}
dvec3 obj::get_norm(size_t f,NORM n){return ns[fs[f][n]-1];}
double obj::get_text(size_t f,TEXT t,AXIS a){return vts[fs[f][t]-1][a];}
dvec3 obj::get_text(size_t f,TEXT t){return vts[fs[f][t]-1];}
double obj::min3(size_t f,AXIS a){return std::min(get_vert(f,V0,a),std::min(get_vert(f,V1,a),get_vert(f,V2,a)));}
double obj::max3(size_t f,AXIS a){return std::max(get_vert(f,V0,a),std::max(get_vert(f,V1,a),get_vert(f,V2,a)));}
triangle obj::get_tri(size_t f){return {get_vert(f,V0),get_vert(f,V1),get_vert(f,V2)};}
