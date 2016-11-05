#include<algorithm>
#include<limits>
#include"prim.hh"

bool ray::hit(const aabb&b,float&in,float&out){
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
  in=std::max(in,0.0f);
  return (out>=in)&&(out>0);
}
bool ray::hit(const plane&pl,vec3&i){
  float a=-dot(pl.n,o-pl.p), b=dot(pl.n,d);
  if(abs(b)<0.001) return false;
  float rt=a/b;
  if(rt<0) return false;
  i=o+rt*d;
  return true;
}
bool ray::hit(const triangle&tr,vec3&i,vec3&c){
  vec3 u=tr.q-tr.p,v=tr.r-tr.p,w;
  plane pl={tr.p,cross(u,v)};
  float uu,vv,uv,wu,wv,D,s,t;
  if(hit(pl,i)){
    uu=dot(u,u); vv=dot(v,v); uv=dot(u,v);
    w=i-tr.p; wu=dot(w,u); wv=dot(w,v);
    D=uv*uv-uu*vv;
    s=(uv*wv-vv*wu)/D;
    if(s<0.0||s>1.0) return false;
    t=(uv*wu-uu*wv)/D;
    if(t<0.0||(s+t)>1.0) return false;
    c={s,t,0};//barycentric coordinates
    return true;
  }else return false;
}
