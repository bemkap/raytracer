#include"ray.hh"

ray::ray(const dvec3&o,const dvec3&a):o(o){
  c2w=lookAt(o,o+a,dvec3(0,1,0));
}
ray::ray(const dvec3&o):ray(o,-o){}
void ray::direct(double x,double y){
  dvec4 pw;
  pw=dvec4(x,y,-1.0,1.0)*c2w;
  d=normalize(dvec3(pw));
  inv.x=1/d.x; inv.y=1/d.y; inv.z=1/d.z;
}
void ray::direct(const dvec3&v){d=v; inv.x=1/d.x; inv.y=1/d.y; inv.z=1/d.z;}
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
bool ray::hit(obj*o,size_t f,dvec3&i,dvec3&c){
  static dvec3 e1,e2;
  static dvec3 P,Q,T;
  double det,inv_det,u,v,t;
  e1=o->vs[o->fs[f][V1]-1]-o->vs[o->fs[f][V0]-1];
  e2=o->vs[o->fs[f][V2]-1]-o->vs[o->fs[f][V0]-1];
  P=cross(d,e2);
  det=dot(e1,P);
  if(abs(det)<0.000001) return false;
  inv_det=1.0/det;
  T=this->o-o->vs[o->fs[f][V0]-1];
  u=dot(T,P)*inv_det;
  if(u<0.0||u>1.0) return false;
  Q=cross(T,e1);
  v=dot(d,Q)*inv_det;
  if(v<0.0||u+v>1.0) return false;
  t=dot(e2,Q)*inv_det;
  if(t>0.0) {
    c={1.0-u-v,u,v};
    i=this->o+t*d;
    return true;
  }
  return false;
}
void saturate(dvec3&I){
  I.x=std::min(255.0,std::max(0.0,I.x));
  I.y=std::min(255.0,std::max(0.0,I.y));
  I.z=std::min(255.0,std::max(0.0,I.z));
}
