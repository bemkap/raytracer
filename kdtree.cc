#include<algorithm>
#include<cmath>
#include<glm/glm.hpp>
#include<limits>
#include<stack>
#include"kdtree.hh"
#include"obj.hh"
#include"prim.hh"
using namespace std;
using namespace glm;

enum method {SPATIAL,SAH};
constexpr int MAX_DEPTH=1;

inline double SA(aabb b){return 2*(b.t[0]-b.f[0])*(b.t[1]-b.f[1])*(b.t[2]-b.f[2]);}
inline double C(double Pl,double Pr,double Nl,double Nr){return Pl*Nl+Pr*Nr;}
double sah(plane&p,aabb&V,double Nl,double Nr,double Np){
  aabb Vl=V,Vr=V;
  Vl.t[p.k]=Vr.f[p.k]=p.e;
  double Pl=SA(Vl)/SA(V);
  double Pr=SA(Vr)/SA(V);
  double Cl=C(Pl,Pr,Nl+Np,Nr);
  double Cr=C(Pl,Pr,Nl,Nr+Np);
  return std::min(Cl,Cr);
}
template<method M>plane find_plane(obj*,unsigned,vector<long>&,aabb,double&);
template<>plane find_plane<SPATIAL>(obj*o,unsigned d,vector<long>&t,aabb b,double&C){
  double mn,mx; mn=mx=f2v(o,t[0],0)[d%3];
  for(auto i:t){
    mn=std::min(mn,min3(o,i,d));
    mx=std::max(mx,max3(o,i,d));
  }
  return {(mn+mx)/2,d%3};
}
template<>plane find_plane<SAH>(obj*o,unsigned d,vector<long>&ts,aabb b,double&C){
  aabb Vl,Vr; plane p_mn,p[2];
  double c, c_mn=std::numeric_limits<double>::infinity();
  for(int k=0; k<3; ++k){
    sort(ts.begin(),ts.end(),[o,k](long a,long b){return min3(o,a,k)<=min3(o,b,k);});
    size_t Nr=0,Np=0,Nl=ts.size();
    for(size_t i=0; i<ts.size(); ++i){
      p[0].k=k; p[0].e=min3(o,ts[i],k);
      p[1].k=k; p[1].e=max3(o,ts[i],k);
      while(i<ts.size()&&)
      
      for(int j=0; j<2; ++j){
        c=sah(p[j],b,i,ts.size()-i,0);
        if(c<c_mn){c_mn=c; p_mn=p[j];}
      }
    }
  }
  // for(auto t:ts){
  //   double c_mn=std::numeric_limits<double>::infinity(),c;
  //   perfect(f2t(o,t),b,ps);
  //   for(int i=0; i<6; ++i){
  //     Vl.t[ps[i].k]=Vr.f[ps[i].k]=ps[i].e;
  //     classify(o,ts,Vl,Vr,ps[i],Tl,Tr,Tp);
  //     c=sah(ps[i],b,Tl,Tr,Tp);
  //     if(c<c_mn){c_mn=c; p_mn=ps[i];}
  //   }
  // }
  return p_mn;
}
kdtree::kdtree(obj*o,aabb b,unsigned d,vector<long>&t):
  depth(d),bounds(b),left(nullptr),right(nullptr){
  double C=0;
  if(d<20&&t.size()>15){
    split=find_plane<SAH>(o,d,t,b,C);
    aabb lb,rb; lb=rb=b;
    lb.t[split.k]=rb.f[split.k]=split.e;
    vector<long> lt,rt;
    for(auto i:t){
      if(max3(o,i,split.k)>=split.e) rt.push_back(i);
      if(min3(o,i,split.k)<=split.e) lt.push_back(i);
    }
    left =new kdtree(o,lb,d+1,lt);
    right=new kdtree(o,rb,d+1,rt);
  }else for(auto i:t) ts.push_back(i);
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
    }
    for(auto i:c.node->ts){
      triangle tr=f2t(o,i);
      if(r.hit(tr,v,bc)){
    	try{n=bc.x*f2n(o,i,0)+bc.y*f2n(o,i,1)+bc.z*f2n(o,i,2);}
    	catch(int e){n=r.o-v;}
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
  return false;
}
