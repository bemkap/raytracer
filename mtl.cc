#include<fstream>
#include<regex>
#include"mtl.hh"

typedef void(*handle)(mat*,const smatch&);
void h_ns(mat*m,const smatch&sm){m->ns=stod(sm[1]);}
void h_ka(mat*m,const smatch&sm){m->ka=dvec3(stod(sm[1]),stod(sm[2]),stod(sm[3]));}
void h_kd(mat*m,const smatch&sm){m->kd=dvec3(stod(sm[1]),stod(sm[2]),stod(sm[3]));}
void h_ks(mat*m,const smatch&sm){m->ks=dvec3(stod(sm[1]),stod(sm[2]),stod(sm[3]));}
void h_ni(mat*m,const smatch&sm){m->ni=stod(sm[1]);}
void h_d(mat*m,const smatch&sm){m->d=stod(sm[1]);}
void h_illum(mat*m,const smatch&sm){m->illum=stod(sm[1]);}
void h_ka_map(mat*m,const smatch&sm){string s(sm[1]); m->ka_map=CImg<double>(s.c_str());}
void h_kd_map(mat*m,const smatch&sm){string s(sm[1]); m->kd_map=CImg<double>(s.c_str());}
void h_ks_map(mat*m,const smatch&sm){string s(sm[1]); m->ks_map=CImg<double>(s.c_str());}

bool mtl::read(string&fn,map<string,mat*>&mm){
  string line;
  ifstream in(fn); mat*cm;
  string r_double="((?:\\+|-)?\\d+(?:\\.\\d+)?(?:[eE]-?\\d+)?)";
  string r_vec3=r_double+"\\s+"+r_double+"\\s+"+r_double;
  regex r_obj[]={
    regex("Ns\\s+"+r_double), regex("Ka\\s+"+r_vec3), regex("Kd\\s+"+r_vec3),
    regex("Ks\\s+"+r_vec3), regex("Ni\\s+"+r_double), regex("d\\s+"+r_double),
    regex("illum\\s+"+r_double), regex("map_Ka\\s+(.+)"), regex("map_Kd\\s+(.+)"),
    regex("map_Ks\\s+(.+)"), regex("newmtl\\s+(.+)")
  };
  handle handles[]={h_ns,h_ka,h_kd,h_ks,h_ni,h_d,h_illum,h_ka_map,h_kd_map,h_ks_map};
  smatch match;
  if(in.rdstate()&ifstream::failbit) return false;
  else{
    while(getline(in,line)){
      int i; for(i=0; i<10&&!regex_match(line,match,r_obj[i]); ++i);
      if(i==10){if(regex_match(line,match,r_obj[i])) mm[match[1]]=cm=new mat;}
      else handles[i](cm,match);
    }
  }
  in.close();
  return true;
}
dvec3 mat::I(vector<light>&ls,dvec3&t,dvec3&i,dvec3&n,dvec3&v,double sh){
  dvec3 I,N=normalize(n); double ia=0;
  double ka_m=1,kd_m=1,ks_m=1;
  if(ka_map.size()>0){
    double Tx=t.x*ka_map.width();
    double Ty=t.y*ka_map.height();
    ka_m=ka_map(Tx,Ty);
  }
  if(kd_map.size()>0){
    double Tx=t.x*kd_map.width();
    double Ty=t.y*kd_map.height();
    kd_m=kd_map(Tx,Ty);
  }
  if(ks_map.size()>0){
    double Tx=t.x*ks_map.width();
    double Ty=t.y*ks_map.height();
    ks_m=ks_map(Tx,Ty);
  }
  for(auto l:ls){
    ia+=l.ia*l.id;
    dvec3 L=normalize(l.p-i);
    double LN=dot(L,N);
    I+=0.9*kd*kd_m*std::max(0.0,LN)*l.id*sh;
    if(illum>1){
      double T=dot(N,normalize(L+(v-i)));
      I+=ks*kd_m*(pow(std::max(0.0,T),ns)*l.is)*sh;
    }
  }
  if(illum>2) I+=ks_m*0.2;
  return I+0.1*ka*ka_m*ia;
}
