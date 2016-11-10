#include<cmath>
#include<cstdio>
#include<fstream>
#include"mtl.hh"

mtl::mtl(string&fn){
  string line; char str[128]={0};
  ifstream in(fn);
  if(in.rdstate()&ifstream::failbit) st=BAD;
  else{
    while(getline(in,line)){
      if(sscanf(line.c_str(),"newmtl %s",str)) name=string(str,strlen(str));
      else if(1==sscanf(line.c_str(),"Ns %lf",&ns));
      else if(3==sscanf(line.c_str(),"Ka %lf %lf %lf",&ka.x,&ka.y,&ka.z));
      else if(3==sscanf(line.c_str(),"Kd %lf %lf %lf",&kd.x,&kd.y,&kd.z));
      else if(3==sscanf(line.c_str(),"Ks %lf %lf %lf",&ks.x,&ks.y,&ks.z));
      else if(1==sscanf(line.c_str(),"Ni %lf",&ni));
      else if(1==sscanf(line.c_str(),"d %lf",&d));
      else if(1==sscanf(line.c_str(),"illum %d",&illum));
    }
    in.close();
    st=GOOD;
  }
}
dvec3 mtl::I(vector<light>&ls,dvec3 i,dvec3 n,dvec3 v){
  dvec3 I,N=normalize(n); double ia=0;
  for(auto l:ls){
    ia+=l.ia;
    dvec3 L=normalize(l.p-i);
    I+=kd*dot(L,N)*l.id;
    dvec3 R=2*dot(L,N)*N-L;
    I+=ks*pow(dot(normalize(R),normalize(v-i)),ns)*l.is;
  }
  return I+ka*ia;
}
