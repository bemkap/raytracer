#include<algorithm>
#include<cmath>
#include<glm/glm.hpp>
#include<stack>
#include"kdtree.hh"
#include"obj.hh"
#include"prim.hh"
using namespace std;
using namespace glm;

enum method {SPATIAL,SAH};
constexpr int MAX_DEPTH=1;

template<method M>plane find_plane(obj*,unsigned,vector<long>&,aabb);
template<>plane find_plane<SPATIAL>(obj*o,unsigned d,vector<long>&t,aabb b){
  plane p;
  p.n[d%3]=1;
  double mn,mx; mn=mx=o->f2v(t[0],0)[d%3];
  for(auto i:t){
    mn=std::min(mn,o->min3(i,d));
    mx=std::max(mx,o->max3(i,d));
  }
  p.p[d%3]=(mn+mx)/2;
  return p;
}
template<>plane find_plane<SAH>(obj*o,unsigned d,vector<long>&t,aabb b){
  long i_mn; double c_mn,C;
  double t_cost=t.size(),i_cost=1;
  for(size_t i=0; i<t.size(); ++i){
    double p=o->max3(t[i],d);
    double l_area=2*(p-b.f[d%3])*(b.t[(d+1)%3]-b.f[(d+1)%3])*(b.t[(d+2)%3]-b.f[(d+2)%3]);
    double r_area=2*(b.t[d%3]-p)*(b.t[(d+1)%3]-b.f[(d+1)%3])*(b.t[(d+2)%3]-b.f[(d+2)%3]);
    size_t l_count=i;
    size_t r_count=t.size()-i;
    C=t_cost+i_cost*(l_area*l_count+r_area*r_count);
    if(C<c_mn){c_mn=C; i_mn=i;}
  }
  plane p;
  p.n[d%3]=1;
  p.p[d%3]=o->max3(t[i_mn],d);
  return p;
}
kdtree::kdtree(obj*o,aabb b,unsigned d,vector<long>&t):
  depth(d),bounds(b),left(nullptr),right(nullptr){
  if(d<20&&t.size()>10){
    split=find_plane<SPATIAL>(o,d,t,b);
    aabb lb,rb; lb=rb=b;
    lb.t[d%3]=rb.f[d%3]=split.p[d%3];
    vector<long> lt,rt;
    for(auto i:t){
      if(o->max3(i,d)>=split.p[d%3]) rt.push_back(i);
      if(o->min3(i,d)<=split.p[d%3]) lt.push_back(i);
    }
    left =new kdtree(o,lb,d+1,lt);
    right=new kdtree(o,rb,d+1,rt);
  }else for(auto i:t) ts.push_back(i);
}
kdtree::~kdtree(){
  if(nullptr!= left) delete  left;
  if(nullptr!=right) delete right;
}
bool kdtree::leafp(){return !(left||right);}
bool kdtree::hit(obj*o,ray r,dvec3&I,vector<light>&ls,int rtd){
  stack<elem> stk;
  elem c; c.node=this;
  if(r.hit(bounds,c.in,c.out)){
    stk.push(c);
    while(!stk.empty()){
      c=stk.top(); stk.pop();
      while(!c.node->leafp()){
        double s=c.node->split.p[c.node->depth%3]-r.o[c.node->depth%3];
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
	triangle tr=o->f2t(i);
	dvec3 v,n,bc;
	if(r.hit(tr,v,bc)){
          try{n=bc.x*o->f2n(i,0)+bc.y*o->f2n(i,1)+bc.z*o->f2n(i,2);}
          catch(int e){n=r.o-v;}
          I+=o->mtls[o->fs[i].m]->I(ls,v,n,r.o);
          if(rtd<MAX_DEPTH){
            dvec3 N=normalize(n), d=r.d-2.0*dot(r.d,N)*N;
            ray s(v+d*0.001); s.d=d;
            hit(o,s,I,ls,rtd+1);
          }
	  return true;
	}
      }
    }
  }
  return false;
}
