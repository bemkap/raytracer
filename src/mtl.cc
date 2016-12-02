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
  dvec3 I(0,0,0);
  double ka_m=0,kd_m=255,ks_m=255;
  if(ka_map.size()>0) ka_m=ka_map(t.x*ka_map.width(),(1-t.y)*ka_map.height());
  if(kd_map.size()>0) kd_m=kd_map(t.x*kd_map.width(),(1-t.y)*kd_map.height());
  if(ks_map.size()>0) ks_m=ks_map(t.x*ks_map.width(),(1-t.y)*ks_map.height());
  for(auto l:ls){
    dvec3 L=l.p-i;
    double d=length(L);
    L*=1.0/d; d*=d;
    double NL=dot(n,L);
    double intensity=std::max(0.0,std::min(1.0,(NL)));
    double Diff=intensity/d;
    dvec3 H=normalize(L+(v-i));
    double NH=dot(n,H);
    intensity=pow(std::max(0.0,std::min(1.0,NH)),ns);
    double Spec=intensity/d;
    I+=l.ia*ka*ka_m+l.id*Diff*kd*kd_m+l.is*Spec*ks*ks_m;
  }
  return I;
}
