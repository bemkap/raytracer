#include<glm/glm.hpp>
#include<limits>
#include<stack>
#include"kdtree.hh"
using namespace std;
using namespace glm;

constexpr int MAX_DEPTH=1;
struct event {double e; unsigned ty; size_t t;};
struct cost {double c; unsigned s;};

inline double SA(aabb b){return 2*((b.t.x-b.f.x)*(b.t.y-b.f.y)+
				   (b.t.y-b.f.y)*(b.t.z-b.f.z)+
				   (b.t.z-b.f.z)*(b.t.x-b.f.x));}
inline double C(double Pl,double Pr,double Nl,double Nr){return 1.5*(Pl*Nl+Pr*Nr);}
cost sah(plane&p,aabb&V,double Nl,double Nr,double Np){
  aabb Vl=V,Vr=V;
  Vl.t[p.k]=Vr.f[p.k]=p.e;
  double SAV=SA(V);
  if(SAV<=0) return {0,2};
  else{
    double Pl=SA(Vl)/SAV;
    double Pr=SA(Vr)/SAV;
    double Cl=C(Pl,Pr,Nl+Np,Nr);
    double Cr=C(Pl,Pr,Nl,Nr+Np);
    if(Cl<Cr) return {Cl*(1-0.2*(Nl==0||Nr==0)),0};
    else return {Cr*(1-0.2*(Nl==0||Nr==0)),1};
  }
}
plane find_plane(obj*o,unsigned d,vector<size_t>&ts,aabb b,double&c_mn,vector<event>&u,unsigned&s){
  aabb Vl,Vr; plane p_mn,p;
  cost c; c_mn=std::numeric_limits<double>::infinity();
  unsigned k=d%3;
  for(size_t i=0; i<ts.size(); ++i){
    double mn=o->min3(ts[i],AXIS(k));
    double mx=o->max3(ts[i],AXIS(k));
    if(mn==mx) u.push_back({mn,1,i});
    else{u.push_back({mn,2,i}); u.push_back({mx,0,i});}
  }
  sort(u.begin(),u.end(),[](event pa,event pb){
      return pa.e<pb.e||(pa.e==pb.e&&pa.ty<pb.ty);
    });
  size_t Nr=ts.size(),Np=0,Nl=0;
  for(size_t i=0; i<u.size(); ){
    p.k=AXIS(k); p.e=u[i].e;
    size_t p0=0,p1=0,p2=0;
    while(i<u.size()&&p.e>=u[i].e&&u[i].ty==0){++i; ++p0;}
    while(i<u.size()&&p.e>=u[i].e&&u[i].ty==1){++i; ++p1;}
    while(i<u.size()&&p.e>=u[i].e&&u[i].ty==2){++i; ++p2;}
    Np=p1; Nr-=p1+p0;
    c=sah(p,b,Nl,Nr,Np);
    if(c.c<c_mn){c_mn=c.c; p_mn=p; s=c.s;}
    Nl+=p1+p2; Np=0;
  }
  return p_mn;
}
kdtree::kdtree(obj*o,aabb b,unsigned d,vector<size_t>&t):
  depth(d),bounds(b),left(nullptr),right(nullptr){
  double C; unsigned s;
  vector<event> u;
  if(d>=20||t.size()<=15) ts=t;
  else{
    aabb lb=b,rb=b;
    split=find_plane(o,d,t,b,C,u,s);    
    lb.t[split.k]=rb.f[split.k]=split.e;
    vector<size_t> lt,rt,ti(t.size(),2);
    for(auto e:u){
      switch(e.ty){
      case 0: if(e.e<=split.e) ti[e.t]=0; break; //end event
      case 2: if(e.e>=split.e) ti[e.t]=1; break; //start event
      case 1: //planar event
        if(e.e<split.e||(e.e==split.e&&s==0)) ti[e.t]=0;
        if(e.e>split.e||(e.e==split.e&&s==1)) ti[e.t]=1;
      }
    }
    for(size_t i=0; i<ti.size(); ++i){
      if(ti[i]!=0) rt.push_back(t[i]);
      if(ti[i]!=1) lt.push_back(t[i]);
    }
    left=new kdtree(o,lb,d+1,lt);
    right=new kdtree(o,rb,d+1,rt);
  }
}
kdtree::~kdtree(){
  if(nullptr!=left) delete left;
  if(nullptr!=right) delete right;
}
bool kdtree::leaf_p(){return !(left||right);}
bool kdtree::hit(obj*o,ray&r,dvec3&I,dvec3&v,vector<light>&ls,int rtd){
  static stack<elem> stk;
  static elem c; c.node=this;
  static dvec3 n,t,bc,bc1,u;
  long ih=-1;
  stk.push(c);
  while(!stk.empty()){
    c=stk.top(); stk.pop();
    if(r.hit(c.node->bounds,c.in,c.out)){
    while(!c.node->leaf_p()){
      double s=c.node->split.e-r.o[c.node->split.k];
      double t=s/r.d[c.node->split.k];
      kdtree*near,*far;
      if(s>=0){near=c.node->left; far=c.node->right;}
      else{near=c.node->right; far=c.node->left;}
      if(t>=c.out||t<0) c.node=near;
      else if(t<=c.in) c.node=far;
      else{
	stk.push({far,t,c.out});
	c.node=near;
	c.out=t;
      }
    }
    for(auto i:c.node->ts)
      if(r.hit(o,i,u,bc1)&&(0>ih||length(u-r.o)<length(v-r.o))){
	v=u; ih=i; bc=bc1;
	break;
      }
    }
  }
  if(ih>-1){
    //shadow
    double sh=1;
    if(rtd<MAX_DEPTH){
      for(auto l:ls){
	dvec3 lv=l.p-v,J;
	ray r1(v+lv*0.001); r1.direct(lv);
	if(hit(o,r1,J,u,ls,MAX_DEPTH)&&length(l.p-u)<length(lv)) sh*=0.5;
      }
      //reflection
      dvec3 d=reflect(r.d,n);
      ray r2(v+d*0.001); r2.direct(d);
      hit(o,r2,I,v,ls,rtd+1);
      //refraction
      d=refract(r.d,n,1.0/1.33);
      r2=ray(v+d*0.001); r2.direct(d);
      hit(o,r2,I,v,ls,rtd+1);
    }
    //color
    if(o->has_ns) n=bc.x*o->ns[o->fs[ih][N0]-1]+bc.y*o->ns[o->fs[ih][N1]-1]+bc.z*o->ns[o->fs[ih][N2]-1];
    else n=r.o-v;
    if(o->has_vts) t=bc.x*o->vts[o->fs[ih][T0]-1]+bc.y*o->vts[o->fs[ih][T1]-1]+bc.z*o->vts[o->fs[ih][T2]-1];
    I+=pow(0.2,rtd)*o->fs[ih].m->I(ls,t,v,n,r.o,sh);
  }
  return ih>-1;
}

