#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"kdtree.h"
#include"vec3.h"

typedef int(*fcmp)(const void*,const void*);

#define FCMP(f,c)					     \
  int f(const void*a,const void*b){			     \
    const tri*ta=a,*tb=b;				     \
    float mpa=(ta->v[0].c+ta->v[1].c+ta->v[2].c)/3;	     \
    float mpb=(tb->v[0].c+tb->v[1].c+tb->v[2].c)/3;	     \
    return mpa-mpb;					     \
  }

FCMP(cx,x); FCMP(cy,y); FCMP(cz,z);

static struct kdtree*kdtree_new_aux(tri*ns,unsigned d,unsigned i,unsigned j){
  if(i>=j) return NULL;
  fcmp cmp[3]={cx,cy,cz};
  qsort(ns+i,j-i,sizeof(tri),cmp[d%3]);
  struct kdtree*t=kdtree_leaf(ns[(i+j-1)/2],d);
  t->bounds.from=ns[i].v[d%3];
  t->bounds.to=ns[j-1].v[d%3];
  t->left=kdtree_new_aux(ns,d+1,i,(i+j-1)/2);
  t->right=kdtree_new_aux(ns,d+1,(i+j-1)/2+1,j);
  return t;
}
struct kdtree*kdtree_new(tri*ns,unsigned sz){
  return kdtree_new_aux(ns,0,0,sz);
}
struct kdtree*kdtree_leaf(tri n,unsigned d){
  struct kdtree*t=malloc(sizeof(struct kdtree));
  t->node=n;
  t->depth=d;
  t->left=t->right=NULL;
  return t;
}
static inline int ondepth(const tri a,const tri b,unsigned d){
  return (a.v[d%3].x+a.v[d%3].y+a.v[d%3].z-b.v[d%3].x-b.v[d%3].y-b.v[d%3].z)/3;
}  
struct kdtree*kdtree_add(struct kdtree*t,tri n){
  if(NULL==t) return kdtree_leaf(n,0);
  if(0<ondepth(n,t->node,t->depth)) t->right=kdtree_add(t->right,n);
  else t->left=kdtree_add(t->left,n);
  if(NULL!=t->right) t->right->depth=t->depth+1;
  if(NULL!=t->left) t->left->depth=t->depth+1;
  return t;
}
//ns tiene que tener suficiente espacio
static unsigned kdtree_collect(tri*ns,struct kdtree*t){
  if(NULL==t) return 0;
  *ns=t->node;
  unsigned i=kdtree_collect(ns+1,t->left);
  return i+kdtree_collect(ns+i,t->right);
}  
struct kdtree*kdtree_remove(struct kdtree*t,tri n){
  if(NULL==t) return NULL;
  if(0<ondepth(n,t->node,t->depth)) t->right=kdtree_remove(t->right,n);
  else if(0>ondepth(n,t->node,t->depth)) t->left=kdtree_remove(t->left,n);
  else{
    unsigned sz=kdtree_count(t);
    tri*ns=malloc(sz*sizeof(tri));
    kdtree_collect(ns,t);
    for(int i=0; i<3; ++i) for(int j=0; j<3; ++j) t->node.v[i].p[j]=INFINITY;
    struct kdtree*s=kdtree_new_aux(ns,t->depth,0,sz);
    kdtree_destroy(t);
    free(ns);
    return s;
  }
  return t;
}
struct kdtree*kdtree_nearest(struct kdtree*t,tri n){
  if(NULL==t) return NULL;
  if(t->node.p.x==INFINITY){kdtree_destroy(t); return NULL;}
  if(0<ondepth(n,t->node,t->depth)) return kdtree_nearest(t->right,n);
  else if(0>ondepth(n,t->node,t->depth)) return kdtree_nearest(t->left,n);
  return t;
}
static inline float max(float a,float b){ return a>b?a:b; }
static inline float min(float a,float b){ return a<b?a:b; }
static inline int signum(float x){ return (x>0)-(x<0); }
static int box_hit(struct box b,struct ray r){
  printf("box hit:\n\tr.p=(%f,%f,%f)\tr.dir=(%f,%f,%f)\n\tb.from=(%f,%f,%f)\tb.to=(%f,%f,%f)\n",
	 r.p0.x,r.p0.y,r.p0.z,r.dir.x,r.dir.y,r.dir.z,b.from.x,b.from.y,b.from.z,b.to.x,b.to.y,b.to.z);
  vec3 v=vec3_sub(b.from,r.p0);
  vec3 w=vec3_sub(b.to,r.p0);
  float x=max(max(signum(r.dir.x)*v.x,signum(r.dir.y)*v.y),signum(r.dir.z)*v.z);
  float y=max(max(signum(r.dir.x)*w.x,signum(r.dir.y)*w.y),signum(r.dir.z)*w.z);
  return x<=y;
}
static int tri_hit(tri tr,struct ray r,vec3*i){//the hell?
  if(tr.p.x==-1.0)
    printf("tri hit:\n\tr.p=(%f,%f,%f)\tr.dir=(%f,%f,%f)\n\ttri.p=(%f,%f,%f)\ttri.q=(%f,%f,%f)\ttri.r=(%f,%f,%f)\n",
	   r.p0.x,r.p0.y,r.p0.z,r.dir.x,r.dir.y,r.dir.z,tr.p.x,tr.p.y,tr.p.z,tr.q.x,tr.q.y,tr.q.z,tr.r.x,tr.r.y,tr.r.z);
  const float SMALL_NUM=0.00000001;
  vec3 u,v,n;
  vec3 w0,w;
  float rt,a,b;
  u=vec3_sub(tr.q,tr.p);
  v=vec3_sub(tr.r,tr.p);
  n=vec3_cross(u,v);
  if(n.x=0&&n.y==0&&n.z==0) return -1;
  w0=vec3_sub(r.p0,tr.p);
  a=-vec3_dot(n,w0);
  b=vec3_dot(n,r.dir);
  if(fabs(b)<SMALL_NUM) return 0==a?2:0;
  rt=a/b;
  if(rt<0.0) return 0;
  *i=vec3_add(r.p0,vec3_mul(rt,r.dir));
  float uu,uv,vv,wu,wv,D;
  uu=vec3_dot(u,u);
  uv=vec3_dot(u,v);
  vv=vec3_dot(v,v);
  w=vec3_sub(*i,tr.p);
  wu=vec3_dot(w,u);
  wv=vec3_dot(w,v);
  D=uv*uv-uu*vv;
  float s,t;
  s=(uv*wv-vv*wu)/D;
  if(s<0.0||s>1.0) return 0;
  t=(uv*wu-uu*wv)/D;
  if(t<0.0||(s+t)>1.0) return 0;
  return 1;
}
int kdtree_hit(struct kdtree*t,struct ray r,vec3*v){
  if(NULL==t) return 0;//||!box_hit(t->bounds,r)) return 0;
  else if(NULL==t->left&&NULL==t->right) return tri_hit(t->node,r,v);
  else return kdtree_hit(t->left,r,v)||kdtree_hit(t->right,r,v);
} 
unsigned kdtree_count(struct kdtree*t){
  return NULL==t?0:1+kdtree_count(t->left)+kdtree_count(t->right);
}
void kdtree_destroy(struct kdtree*t){
  if(NULL==t) return;
  kdtree_destroy(t->left);
  kdtree_destroy(t->right);
  free(t);
}
