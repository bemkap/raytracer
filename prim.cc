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
  d=normalize(dvec3(pw));
  inv.x=1/d.x; inv.y=1/d.y; inv.z=1/d.z;
}
void ray::direct(dvec3&v){d=v; inv.x=1/d.x; inv.y=1/d.y; inv.z=1/d.z;}
bool ray::hit(const aabb&b,double&in,double&out){
  double t1=(b.f[0]-o[0])*inv[0];
  double t2=(b.t[0]-o[0])*inv[0];
  in=std::min(t1,t2);
  out=std::max(t1,t2);
  for(int i=1; i<3; ++i){
    t1=(b.f[i]-o[i])*inv[i];
    t2=(b.t[i]-o[i])*inv[i];
    in =std::max( in,std::min(t1,t2));
    out=std::min(out,std::max(t1,t2));
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
  // dvec3 e1,e2;
  // dvec3 P,Q,T;
  // double det,inv_det,u,v,t;
  // e1=tr.q-tr.p;
  // e2=tr.r-tr.q;
  // P=cross(d,e2);
  // det=dot(e1,P);
  // if(abs(det)<0.001) return false;
  // inv_det=1.0/det;
  // T=o-tr.p;
  // u=dot(T,P)*inv_det;
  // if(u<0.0||u>1.0) return false;
  // Q=cross(T,e1);
  // v=dot(d,Q)*inv_det;
  // if(v<0.0||u+v>1.0) return false;
  // t=dot(e2,Q)*inv_det;
  // if(t>0.001) {
  //   c={u,v,1.0-u-v};
  //   i=o+t*d;
  //   return true;
  // }
  // return false;
}
void saturate(dvec3&I){
  I.x=std::min(1.0,std::max(0.0,I.x));
  I.y=std::min(1.0,std::max(0.0,I.y));
  I.z=std::min(1.0,std::max(0.0,I.z));
}
