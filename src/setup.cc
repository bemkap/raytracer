#include"setup.hh"

typedef void(*handle)(setup*,smatch&);
void h_origin(setup*s,smatch&sm){s->r=ray({stod(sm[1]),stod(sm[2]),stod(sm[3])});}
void h_light(setup*s,smatch&sm){
  light l; l.p=dvec3(stod(sm[1]),stod(sm[2]),stod(sm[3])); l.ia=stod(sm[4]); l.id=stod(sm[5]); l.is=stod(sm[6]);
  s->ls.push_back(l);
}
void h_in(setup*s,smatch&sm){s->o=new obj(sm[1]);}
void h_out(setup*s,smatch&sm){s->out.open(sm[1]);}
void h_nop(setup*s,smatch&sm){}

setup::setup(const string&cfg_fn,int width,int height){
  ifstream in(cfg_fn);
  string line;
  string r_double="((?:\\+|-)?\\d+(?:\\.\\d+)?(?:[eE]-?\\d+)?)";
  string r_vec3=r_double+"\\s+"+r_double+"\\s+"+r_double;
  string r_vec2=r_double+"\\s+"+r_double;
  regex r_obj[]={
    regex("origin="+r_vec3), regex("light="+r_vec3+"\\s+"+r_vec3),
    regex("in=(.+)"), regex("out=(.+)")
  };
  handle handles[]={h_origin,h_light,h_in,h_out,h_nop};
  smatch match;
  if(!(in.rdstate()&ifstream::failbit))
    while(getline(in,line)){
      int i; for(i=0; i<4&&!regex_match(line,match,r_obj[i]); ++i);
      handles[i](this,match);
    }
  in.close();
  aabb b;
  vector<size_t> ts(o->fs.size());
  for(size_t i=0; i<ts.size(); ++i) ts[i]=i;
  for(auto v:o->vs)
    for(int i=0; i<3; ++i){
      b.f[i]=std::min(b.f[i],v[i]);
      b.t[i]=std::max(b.t[i],v[i]);
    }
  t=new kdtree(o,b,0,ts);
  scr=new unsigned char[height*width*3];
}
setup::~setup(){
  delete t;
  if(o) delete o;
  out.close();
  delete[] scr;
}
