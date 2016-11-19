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
constexpr int MAX_DEPTH=0;

inline double SA(aabb b){return 2*(b.t[0]-b.f[0])*(b.t[1]-b.f[1])*(b.t[2]-b.f[2]);}
inline double C(double Pl,double Pr,double Nl,double Nr){return Pl*Nl+Pr*Nr;}
inline void perfect(triangle tr,aabb b,plane ps[6]){
  ps[0].e=std::max(b.f.x,std::min(tr.p.x,std::min(tr.q.x,tr.r.x)));
  ps[1].e=std::min(b.t.x,std::max(tr.p.x,std::max(tr.q.x,tr.r.x)));
  ps[2].e=std::max(b.f.y,std::min(tr.p.y,std::min(tr.q.y,tr.r.y)));
  ps[3].e=std::min(b.f.y,std::max(tr.p.y,std::max(tr.q.y,tr.r.y)));
  ps[4].e=std::max(b.f.z,std::min(tr.p.z,std::min(tr.q.z,tr.r.z)));
  ps[5].e=std::min(b.f.z,std::max(tr.p.z,std::max(tr.q.z,tr.r.z)));
}

double SAH(plane&p,aabb&V,double Nl,double Nr,double Np){
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
  double mn,mx; mn=mx=o->f2v(t[0],0)[d%3];
  for(auto i:t){
    mn=std::min(mn,o->min3(i,d));
    mx=std::max(mx,o->max3(i,d));
  }
  return {(mn+mx)/2,d%3};
}
template<>plane find_plane<SAH>(obj*o,unsigned d,vector<long>&ts,aabb b,double&C){
  // double c_mn=std::numeric_limits<double>::infinity(),sp=0;
  // double p[2],l_P,r_P;
  // aabb bl=b,br=b;
  // sort(ts.begin(),ts.end(),[o,d](long a,long b){return o->min3(a,d%3)<o->min3(b,d%3);});
  // for(size_t i=0; i<ts.size(); ++i){
  //   p[0]=o->min3(ts[i],d);
  //   p[1]=o->max3(ts[i],d);
  //   for(int j=0; j<2; ++j){
  //     bl.t[d%3]=br.f[d%3]=p[j];
  //     l_P=SA(bl)/SA(b);
  //     r_P=SA(br)/SA(b);
  //     C=SA(b)==0?0:0.3*(l_P*i+r_P*(ts.size()-i));
  //     if(C<c_mn){c_mn=C; sp=p[j];}
  //   }
  // }
  // C=c_mn;
  // return {sp,d%3};
  aabb Vl,Vr; plane p_mn;
  vector<long> Tl,Tr,Tp; plane ps[6];
  ps[0].k=ps[1].k=0; ps[2].k=ps[3].k=0; ps[4].k=ps[5].k=2;
  for(auto t:ts){
    double c_mn=std::numeric_limits<double>::infinity(),c;
    for(auto p:perfect(o->f2t(t),b)){
      Vl.t[p.k]=Vr.f[p.k]=p.e;
      classify(o,ts,Vl,Vr,p,Tl,Tr,Tp);
      c=SAH(p,b,Tl.size(),Tr.size(),Tp.size());
      if(c<c_mn){c_mn=c; p_mn=p;}
    }
  }
  return p;
}
kdtree::kdtree(obj*o,aabb b,unsigned d,vector<long>&t):
  depth(d),bounds(b),left(nullptr),right(nullptr){
  double C=0;
  if(d<20&&t.size()>15){
    split=find_plane<SPATIAL>(o,d,t,b,C);
    aabb lb,rb; lb=rb=b;
    lb.t[d%3]=rb.f[d%3]=split.e;
    vector<long> lt,rt;
    for(auto i:t){
      if(o->max3(i,split.k)>=split.e) rt.push_back(i);
      if(o->min3(i,split.k)<=split.e) lt.push_back(i);
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
      triangle tr=o->f2t(i);
      if(r.hit(tr,v,bc)){
    	try{n=bc.x*o->f2n(i,0)+bc.y*o->f2n(i,1)+bc.z*o->f2n(i,2);}
    	catch(int e){n=r.o-v;}
    	double sh=1;
    	for(auto l:ls){
    	  dvec3 lv=l.p-v,J,w;
    	  ray r1(v+lv*0.001); r1.direct(lv);
    	  if(hit(o,r1,J,w,ls,MAX_DEPTH)&&length(l.p-w)<length(lv)) sh=0;
    	}
    	I+=pow(0.2,rtd)*o->fs[i].m->I(ls,v,n,r.o,sh);
    	if(rtd<MAX_DEPTH){
    	  dvec3 N=normalize(n),d=r.d-2.0*dot(r.d,N)*N;
    	  ray r2(v+d*0.001); r2.d=d;
    	  hit(o,r2,I,v,ls,rtd+1);
    	}
    	return true;
     }
    }
  }
  return false;
}
