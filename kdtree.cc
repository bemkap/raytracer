#include<iostream>
#include<algorithm>
#include<cmath>
#include<glm/glm.hpp>
#include"kdtree.hh"
#include"parser.hh"
#include"prim.hh"
using namespace std;
using namespace glm;

static plane findplane(obj_desc*o,unsigned d,vector<long>&t){
  plane p;
  p.n[d%3]=1;
  float mn,mx;
  mn=mx=o->f2v(t[0],0)[d%3];
  for(auto i:t){
    mn=std::min(mn,std::min(o->f2v(i,0)[d%3],std::min(o->f2v(i,1)[d%3],o->f2v(i,2)[d%3])));
    mx=std::max(mx,std::max(o->f2v(i,0)[d%3],std::max(o->f2v(i,1)[d%3],o->f2v(i,2)[d%3])));
  }
  p.p[d%3]=(mn+mx)/2;
  return p;
}
kdtree::kdtree(obj_desc*o,box b,unsigned d,vector<long>&t):
  depth(d),bounds(b),left(nullptr),right(nullptr){
  if(d<20&&t.size()>25){
    split=findplane(o,d,t);
    box lb,rb;
    lb.t[d%3]=rb.f[d%3]=split.p[d%3];
    vector<long> lt,rt;
    for(auto i:t)
      if((o->f2v(i,0)[d%3]+o->f2v(i,1)[d%3]+o->f2v(i,2)[d%3])/3<split.p[d%3])
	lt.push_back(i);
      else
	rt.push_back(i);
    left =new kdtree(o,lb,d+1,lt);
    right=new kdtree(o,rb,d+1,rt);
  }else for(auto i:t) ts.push_back(i);
}
kdtree::~kdtree(){
  if(nullptr!= left) delete  left;
  if(nullptr!=right) delete right;
}
bool kdtree::hit(obj_desc*o,ray r,vec3&v,vec3&n){
  float in,out;
  if(!r.hit(bounds,in,out)) return false;
  trav.push({this,in,out});
  while(!trav.empty()){
    elem c=trav.top(); trav.pop();
    while(c.node&&(!c.node->leafp())){
      float s=split.p[c.node->depth%3]-r.p[c.node->depth%3];
      float t=s/r.d[c.node->depth%3];
      kdtree*near,*far;
      if(s<0){near=left; far=right;}
      else{near=right; far=left;}
      if(t>=out||t<0) c.node=near;
      else if(t<=in) c.node=far;
      else{
	trav.push({far,t,out});
	c.node=near;
	out=t;
      }
    }
    if(c.node->leafp())
      for(auto i:ts){
	triangle tr=o->f2t(i);
	vec3 uv;
	if(r.hit(tr,v,uv)){
	  vec3 n0=o->f2n(i,0);
	  vec3 n1=o->f2n(i,1);
	  vec3 n2=o->f2n(i,2);
	  n=uv.z*n0+(1-uv.y-uv.z)*n1+uv.y*n2;
	  return true;
	}
      }
  }
  return false;
}
bool kdtree::leafp(){return !(left||right);}
