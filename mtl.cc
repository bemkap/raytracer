#include<cmath>
#include<cstdio>
#include<fstream>
#include"mtl.hh"

bool mtl::read(string&fn,map<string,mat*>&mm){
  string line; char str[128]={0};
  ifstream in(fn); mat*cm;
  if(in.rdstate()&ifstream::failbit) return false;
  else{
    while(getline(in,line)){
      if(1==sscanf(line.c_str(),"newmtl %s",str)) mm[str]=cm=new mat;
      else if(1==sscanf(line.c_str(),"Ns %lf",&cm->ns));
      else if(3==sscanf(line.c_str(),"Ka %lf %lf %lf",&cm->ka.x,&cm->ka.y,&cm->ka.z));
      else if(3==sscanf(line.c_str(),"Kd %lf %lf %lf",&cm->kd.x,&cm->kd.y,&cm->kd.z));
      else if(3==sscanf(line.c_str(),"Ks %lf %lf %lf",&cm->ks.x,&cm->ks.y,&cm->ks.z));
      else if(1==sscanf(line.c_str(),"Ni %lf",&cm->ni));
      else if(1==sscanf(line.c_str(),"d %lf",&cm->d));
      else if(1==sscanf(line.c_str(),"illum %d",&cm->illum));
    }
  }
  in.close();
  return true;
}
dvec3 mat::I(vector<light>&ls,dvec3&i,dvec3&n,dvec3&v,double sh){
  dvec3 I,N=normalize(n); double ia=0;
  for(auto l:ls){
    ia+=l.ia*l.id;
    dvec3 L=normalize(l.p-i);
    double LN=dot(L,N);
    I+=0.9*kd*std::max(0.0,LN)*l.id*sh;
    if(illum>1){
      double T=dot(N,normalize(L+(v-i)));
      I+=ks*(pow(std::max(0.0,T),ns)*l.is)*sh;
    }
  }
  if(illum>2) I+=ks*0.2;
  return I+0.1*ka*ia;
}
