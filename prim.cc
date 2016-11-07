#include<algorithm>
#include<limits>
#include"prim.hh"

ray::ray(dvec3 o,dvec3 n):o(o),n(n){
  dvec3 N=normalize(n-dvec3(1,0,0));
  n1.x=acos(N.x);
  n1.y=acos(N.y);
  n1.z=acos(N.z);
  l=length(n);
}
void ray::direct(double i,double j){
  d={l,i,j};
  d=rotate(d,n1.x,{1,0,0});
  d=rotate(d,n1.y,{0,1,0});
  d=rotate(d,n1.z,{0,0,1});
}
bool ray::hit(const aabb&b,double&in,double&out){
  in=-std::numeric_limits<double>::infinity();
  out=std::numeric_limits<double>::infinity();
  for(int i=0; i<3; ++i){
    if(d[i]!=0){
      double t1=(b.f[i]-o[i])/d[i];
      double t2=(b.t[i]-o[i])/d[i];
      in =std::max( in,std::min(t1,t2));
      out=std::min(out,std::max(t1,t2));
    }else if(o[i]<=b.f[i]||o[i]>=b.t[i])
      return false;
  }
  return (out>=in)&&(out>0);
}
bool ray::hit(const plane&pl,dvec3&i){
  double a=-dot(pl.n,o-pl.p), b=dot(pl.n,d);
  if(abs(b)<0.001) return false;
  double rt=a/b;
  if(rt<0) return false;
  i=o+rt*d;
  return true;
}
bool ray::hit(const triangle&tr,dvec3&i,dvec3&c){
  dvec3 u=tr.q-tr.p,v=tr.r-tr.p,w;
  plane pl={tr.p,cross(u,v)};
  double uu,vv,uv,wu,wv,D,s,t;
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
