#include<algorithm>
#include<limits>
#include"prim.hh"

ray::ray(dvec3 o,dvec3 a):o(o){
  c2w=lookAt(o,o+a,dvec3(0,1,0));
}
ray::ray(dvec3 o):ray(o,-o){}
void ray::direct(double x,double y){
  dvec4 pw;
  pw=dvec4(x,y,-1.0,1.0)*c2w;
  d =normalize(dvec3(pw));
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
    c={1.0-s-t,s,t};//barycentric coordinates
    return true;
  }else return false;
}
