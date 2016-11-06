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
      else if(1==sscanf(line.c_str(),"Ns %f",&ns));
      else if(3==sscanf(line.c_str(),"Ka %f %f %f",&ka.x,&ka.y,&ka.z));
      else if(3==sscanf(line.c_str(),"Kd %f %f %f",&kd.x,&kd.y,&kd.z));
      else if(3==sscanf(line.c_str(),"Ks %f %f %f",&ks.x,&ks.y,&ks.z));
      else if(1==sscanf(line.c_str(),"Ni %f",&ni));
      else if(1==sscanf(line.c_str(),"d %f",&d));
      else if(1==sscanf(line.c_str(),"illum %d",&illum));
    }
    in.close();
    st=GOOD;
  }
}
vec3 mtl::I(vector<light>&ls,vec3 i,vec3 n,vec3 v){
  vec3 I,N=normalize(n); float ia=0;
  for(auto l:ls){
    ia+=l.ia;
    vec3 L=normalize(l.p-i);
    I+=kd*dot(L,N)*l.id;
    vec3 R=2*dot(L,N)*N-L;
    I+=ks*pow(dot(R,normalize(v-i)),ns)*l.is;
  }
  return I+ka*ia;
}
