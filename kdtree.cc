#include<cmath>
#include<algorithm>
#include<glm/glm.hpp>
#include"kdtree.hh"
#include"parser.hh"
using namespace std;

kdtree::kdtree(obj_desc&o,box b,unsigned d,long i,long j):
  i(i),j(j),depth(d),bounds(b),left(nullptr),right(nullptr){
  if(j-i>5||d<20){
    box bl,br; bl=br=b;
    auto cmp=[o,d](face a,face b){
      float mpa=std::min(std::min(o.vs[a.v0-1][d%3],o.vs[a.v1-1][d%3]),o.vs[a.v2-1][d%3]);
      float mpb=std::min(std::min(o.vs[b.v0-1][d%3],o.vs[b.v1-1][d%3]),o.vs[b.v2-1][d%3]);
      return mpa<mpb;
    };
    sort(o.fs.begin()+i,o.fs.begin()+j,cmp);
    bl.t[d%3]
      =std::min(std::min(o.vs[o.fs[(i+j)/2].v0-1][d%3],
                         o.vs[o.fs[(i+j)/2].v1-1][d%3]),
                o.vs[o.fs[(i+j)/2].v2-1][d%3]);
    br.f[d%3]
      =std::min(std::min(o.vs[o.fs[(i+j)/2+1].v0-1][d%3],
                         o.vs[o.fs[(i+j)/2+1].v1-1][d%3]),
                o.vs[o.fs[(i+j)/2+1].v2-1][d%3]);
    left =new kdtree(o,bl,d+1,i,(i+j)/2);
    right=new kdtree(o,br,d+1,(i+j)/2+1,j);
  }
}
kdtree::kdtree(obj_desc&o,box b){
  kdtree(o,b,0,0,o.fs.size()-1);
}
kdtree::~kdtree(){
  if(left) delete left;
  if(right) delete right;
}
static int box_hit(box b,ray r){
  float tmin=-INFINITY,tmax=INFINITY;
  for(int i=0; i<3; ++i){
    if(r.d[i]==0&&(r.p[i]<b.f[i]||r.p[i]>b.t[i])) return 0;
    else if(r.d[i]!=0){
      float t1=(b.f[i]-r.p[i])/r.d[i];
      float t2=(b.t[i]-r.p[i])/r.d[i];
      tmin=std::max(tmin,std::min(t1,t2));
      tmax=std::min(tmax,std::max(t1,t2));
      if(tmin>tmax||tmax<0) return 0;
    }      
  }
  return tmax>=tmin;
}
static bool tri_hit(tri tr,ray r,vec3&i,vec3&c){
  vec3 u,v,n,w0,w;
  float rt,a,b;
  float uu,uv,vv,wu,wv,D;
  float s,t;
  //plane
  u=tr.q-tr.p; v=tr.r-tr.p;
  n=cross(u,v);
  if((0==n.x)&&(0==n.y)&&(0==n.z)) return false;
  w0=r.p-tr.p;
  a=-dot(n,w0); b=dot(n,r.d);
  if(abs(b)<0.001) return false;
  rt=a/b;
  if(rt<0) return false;
  i=r.p+rt*r.d;
  //triangle
  uu=dot(u,u); vv=dot(v,v); uv=dot(u,v);
  w=i-tr.p; wu=dot(w,u); wv=dot(w,v);
  D=uv*uv-uu*vv;
  s=(uv*wv-vv*wu)/D;
  if(s<0.0||s>1.0) return false;
  t=(uv*wu-uu*wv)/D;
  if(t<0.0||(s+t)>1.0) return false;
  c={0,s,t};//barycentric coordinates
  return true;
}
bool kdtree::hit(obj_desc&o,ray r,vec3&v,vec3&n){
  if(!box_hit(bounds,r)) return false;
  else if((nullptr==left)&&(nullptr==right)){
    for(long a=i; a<j; ++a){
      tri tr={o.vs[o.fs[a].v0-1],o.vs[o.fs[a].v1-1],o.vs[o.fs[a].v2-1]};
      vec3 uv;
      if(tri_hit(tr,r,v,uv)){
        vec3 n0=o.ns[o.fs[a].n0-1];
        vec3 n1=o.ns[o.fs[a].n1-1];
        vec3 n2=o.ns[o.fs[a].n2-1];
        n=uv.z*n0+(1-uv.y-uv.z)*n1+uv.y*n2;
        return true;
      }
    }; return false;
  }else{
    bool le=left?left->hit(o,r,v,n):false;
    bool ri=right?right->hit(o,r,v,n):false;
    return le||ri;
  }
}

