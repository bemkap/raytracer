#include<algorithm>
#include<glm/glm.hpp>
#include<limits>
#include<stack>
#include"kdtree.hh"
#include"obj.hh"
#include"prim.hh"
using namespace std;
using namespace glm;

constexpr int MAX_DEPTH=0;
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
plane find_plane(obj*o,unsigned d,vector<long>&ts,aabb b,double&c_mn,vector<event>&u,unsigned&s){
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
      return pa.e<pb.e||((pa.e==pb.e)&&(pa.ty<pb.ty));
    });
  size_t Nr=ts.size(),Np=0,Nl=0;
  for(size_t i=0; i<u.size(); ++i){
    p.k=AXIS(k); p.e=u[i].e;
    size_t p0=0,p1=0,p2=0;
    while(i<u.size()&&p.e>=u[i].e&&u[i].ty==0){++i; ++p0;}
    while(i<u.size()&&p.e>=u[i].e&&u[i].ty==1){++i; ++p1;}
    while(i<u.size()&&p.e>=u[i].e&&u[i].ty==2){++i; ++p2;}
    Np=p1; Nr-=p1+p0;
    c=sah(p,b,Nl,Nr,Np);
    if(c.c<c_mn){c_mn=c.c; p_mn=p; s=c.s;}
    Nl+=p2+p1; Np=0;
  }
  return p_mn;
}
kdtree::kdtree(obj*o,aabb b,unsigned d,vector<long>&t):
  depth(d),bounds(b),left(nullptr),right(nullptr){
  double C; unsigned s;
  vector<event> u;
  split=find_plane(o,d,t,b,C,u,s);
  if(d>=20||t.size()<=15) for(auto i:t) ts.push_back(i);
  else{
    aabb lb,rb; lb=rb=b;
    lb.t[split.k]=rb.f[split.k]=split.e;
    vector<long> lt,rt;
    // for(auto i:t){
    //   double mn=o->min3(i,split.k);
    //   double mx=o->max3(i,split.k);
    //   if(mx>=split.e) rt.push_back(i);
    //   if(mn<=split.e) lt.push_back(i);
    // }
    vector<long> ti;
    for(auto i:t) ti.push_back(2);
    for(auto e:u){
      if(e.ty==0&&e.e<=split.e) ti[e.t]=0;
      else if(e.ty==2&&e.e>=split.e) ti[e.t]=1;
      else if(e.ty==1){
        if(e.e<split.e||(e.e==split.e&&s==0)) ti[e.t]=0;
        if(e.e>split.e||(e.e==split.e&&s==1)) ti[e.t]=1;
      }
    }
    for(size_t i=0; i<ti.size(); ++i){
      if(ti[i]==0) lt.push_back(t[i]);
      else if(ti[2]==1) rt.push_back(t[i]);
      else{lt.push_back(t[i]); rt.push_back(t[i]);}
    }      
    left =new kdtree(o,lb,d+1,lt);
    right=new kdtree(o,rb,d+1,rt);
  }
}
kdtree::~kdtree(){
  if(nullptr!= left) delete  left;
  if(nullptr!=right) delete right;
}
bool kdtree::leaf_p(){return !(left||right);}
bool kdtree::hit(obj*o,ray r,dvec3&I,dvec3&v,vector<light>&ls,int rtd){
  static stack<elem> stk;
  elem c; c.node=this;
  dvec3 n,bc;
  stk.push(c);
  while(!stk.empty()){
    c=stk.top(); stk.pop();
    if(r.hit(c.node->bounds,c.in,c.out)){
      while(!c.node->leaf_p()){
        double s=c.node->split.e-r.o[c.node->depth%3];
    	double t=s/r.d[c.node->depth%3];
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
      for(auto i:c.node->ts){
	triangle tr=o->get_tri(i);
	if(r.hit(tr,v,bc)){
	  n=bc.x*o->get_norm(i,N0)+bc.y*o->get_norm(i,N1)+bc.z*o->get_norm(i,N2);
	  double sh=1;
	  if(rtd<MAX_DEPTH)
	    for(auto l:ls){
	      dvec3 lv=l.p-v,J,w;
	      ray r1(v+lv*0.001); r1.direct(lv);
	      if(hit(o,r1,J,w,ls,MAX_DEPTH)&&length(l.p-w)<length(lv)) sh=0;
	    }
	  I+=pow(0.1,rtd)*o->fs[i].m->I(ls,v,n,r.o,sh);
	  if(rtd<MAX_DEPTH){
	    dvec3 N=normalize(n),d=r.d-2.0*dot(r.d,N)*N;
	    ray r2(v+d*0.001); r2.direct(d);
	    hit(o,r2,I,v,ls,rtd+1);
	  }
	  return true;
	}
      }
    }
  }
  return false;
}
