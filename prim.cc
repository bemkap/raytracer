#include<algorithm>
#include<limits>
#include"prim.hh"

bool ray::hit(const box&b,float&in,float&out){
  in=-std::numeric_limits<float>::infinity();
  out=std::numeric_limits<float>::infinity();
  for(int i=0; i<3; ++i){
    if(d[i]==0&&(p[i]<b.f[i]||p[i]>b.t[i])) return 0;
    else if(d[i]!=0){
      float t1=(b.f[i]-p[i])/d[i];
      float t2=(b.t[i]-p[i])/d[i];
      in =std::max( in,std::min(t1,t2));
      out=std::min(out,std::max(t1,t2));
      if(in>out||out<0) return 0;
    }
  }
  return out>=in;
}
bool ray::hit(const plane&pl,vec3&i){
  float a,b,rt;
  a=-dot(pl.n,pl.p-p); b=dot(pl.n,d);
  if(abs(b)<0.001) return false;
  rt=a/b;
  if(rt<0) return false;
  i=p+rt*d;
  return true;
}
bool ray::hit(const triangle&t,vec3&i,vec3&c){
  plane p={t.p,cross(t.p,t.q)};
  vec3 u=t.q-t.p,v=t.r-t.p,w;
  float uu,vv,uv,wu,wv,D,r,s;
  if(hit(p,i)){
    uu=dot(u,u); vv=dot(v,v); uv=dot(u,v);
    w=i-t.p; wu=dot(w,u); wv=dot(w,v);
    D=uv*uv-uu*vv;
    r=(uv*wv-vv*wu)/D;
    if(r<0.0||r>1.0) return false;
    s=(uv*wu-uu*wv)/D;
    if(s<0.0||(r+s)>1.0) return false;
    c={0,r,s};//barycentric coordinates
    return true;
  }else return false;
}
