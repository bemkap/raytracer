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

template<method M>plane findplane(obj*,unsigned,vector<long>&);
template<>plane findplane<SPATIAL>(obj*o,unsigned d,vector<long>&t){
  plane p;
  p.n[d%3]=1;
  float mn,mx; mn=mx=o->f2v(t[0],0)[d%3];
  for(auto i:t){
    mn=std::min(mn,o->min3(i,d));
    mx=std::max(mx,o->max3(i,d));
  }
  p.p[d%3]=(mn+mx)/2;
  return p;
}
template<>plane findplane<SAH>(obj*o,unsigned d,vector<long>&t){
  plane p; return p;
}
kdtree::kdtree(obj*o,aabb b,unsigned d,vector<long>&t):
  depth(d),bounds(b),left(nullptr),right(nullptr){
  if(d<20&&t.size()>10){
    split=findplane<SPATIAL>(o,d,t);
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
bool kdtree::hit(obj*o,ray r,vec3&v,vec3&n){
  stack<elem> stk;
  elem c; c.node=this;
  if(r.hit(bounds,c.in,c.out)){
    stk.push(c);
    while(!stk.empty()){
      c=stk.top(); stk.pop();
      while(!c.node->leafp()){
	float t=(split.p[c.node->depth%3]-r.o[c.node->depth%3])/
	  r.d[c.node->depth%3];
	kdtree*near,*far;
	if(t>=0){near=c.node->left; far=c.node->right;}
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
	vec3 uv;
	if(r.hit(tr,v,uv)){
	  vec3 n0=o->f2n(i,0);
	  vec3 n1=o->f2n(i,1);
	  vec3 n2=o->f2n(i,2);
	  n=(1-uv.x-uv.y)*n0+uv.x*n1+(1-uv.x)*n2;
	  return true;
	}
      }
    }
  }
  return false;
}
bool kdtree::leafp(){return !(left||right);}
