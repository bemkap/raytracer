#include<algorithm>
#include<limits>
#include"prim.hh"

bool ray::hit(const box&b,float&in,float&out){
  in=-std::numeric_limits<float>::infinity();
  out=std::numeric_limits<float>::infinity();
  for(int i=0; i<3; ++i){
    if(d[i]!=0){
      float t1=(b.f[i]-o[i])/d[i];
      float t2=(b.t[i]-o[i])/d[i];
      in =std::max( in,std::min(t1,t2));
      out=std::min(out,std::max(t1,t2));
    }else if(o[i]<=b.f[i]||o[i]>=b.t[i])
      return false;
  }
  return (out>in)&&(out>0);
}
bool ray::hit(const plane&pl,vec3&i){
  float a,b,rt;
  a=-dot(pl.n,pl.p-o); b=dot(pl.n,d);
  if(abs(b)<0.001) return false;
  rt=a/b;
  if(rt<0) return false;
  i=o+rt*d;
  return true;
}
bool ray::hit(const triangle&t,vec3&i,vec3&c){
  vec3 u=t.q-t.p,v=t.r-t.p,w;
  plane p={t.p,cross(u,v)};
  float uu,vv,uv,wu,wv,D,r,s;
  if(hit(p,i)){
    uu=dot(u,u); vv=dot(v,v); uv=dot(u,v);
    w=i-t.p; wu=dot(w,u); wv=dot(w,v);
    D=uv*uv-uu*vv;
    r=(uv*wv-vv*wu)/D;
    if(r<0.0||r>1.0) return false;
    s=(uv*wu-uu*wv)/D;
    if(s<0.0||(r+s)>1.0) return false;
    c={r,s,0};//barycentric coordinates
    return true;
  }else return false;
}
