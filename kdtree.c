#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"kdtree.h"
#include"vec3.h"
#include"parser.h"

#define FCMP(f,c)                                                       \
  gint f(const void*a,const void*b,void*ex){                            \
    const face*fa=*(face**)a,*fb=*(face**)b;                            \
    vec3**vs=ex;                                                        \
    float mpa=(vs[fa->v0]->c+vs[fa->v1]->c+vs[fa->v2]->c)/3;            \
    float mpb=(vs[fb->v0]->c+vs[fb->v1]->c+vs[fb->v2]->c)/3;            \
    return mpb-mpa;                                                     \
  }
FCMP(cx,x); FCMP(cy,y); FCMP(cz,z);

#define F2V(o,i,f) (*(vec3*)(o->vs->ar[((face*)o->fs->ar[(i)])->f]))
static struct kdtree*kdtree_new_aux(struct obj_desc*o,unsigned d,long i,long j){
  if(i>=j) return NULL;
  GCompareDataFunc cmp[3]={cx,cy,cz};
  vec3 from,to;
  for(unsigned n=0; n<3; ++n){
    unsigned k=d+2-n;
    g_qsort_with_data(o->fs->ar+i,j-i+1,sizeof(face*),cmp[k%3],o->vs->ar);
    from.p[k%3]=F2V(o,i,a[3*(k%3)]).p[k%3];
    to.p[k%3]=F2V(o,j,a[3*(k%3)]).p[k%3];
  }
  tri f; f.p=F2V(o,(i+j)/2,v0); f.q=F2V(o,(i+j)/2,v1); f.r=F2V(o,(i+j)/2,v2);
  struct kdtree*t=kdtree_leaf(f,d);
  t->bounds.from=from;
  t->bounds.to=to;
  t->left=kdtree_new_aux(o,d+1,i,(i+j)/2-1);
  t->right=kdtree_new_aux(o,d+1,(i+j)/2+1,j);
  return t;
}
struct kdtree*kdtree_new(struct obj_desc*o,unsigned sz){
  return kdtree_new_aux(o,0,0,sz-1);
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
int box_hit(struct box b,struct ray r){
  float tmin=-INFINITY,tmax=INFINITY;
  if(r.dir.x!=0){
    float tx1=(b.from.x-r.p0.x)/r.dir.x;
    float tx2=(b.to.x-r.p0.x)/r.dir.x;
    tmin=max(tmin,min(tx1,tx2));
    tmax=min(tmax,max(tx1,tx2));
  }
  if(r.dir.y!=0){
    float ty1=(b.from.y-r.p0.y)/r.dir.y;
    float ty2=(b.to.y-r.p0.y)/r.dir.y;
    tmin=max(tmin,min(ty1,ty2));
    tmax=min(tmax,max(ty1,ty2));
  }
  if(r.dir.z!=0){
    float tz1=(b.from.z-r.p0.z)/r.dir.z;
    float tz2=(b.to.z-r.p0.z)/r.dir.z;
    tmin=max(tmin,min(tz1,tz2));
    tmax=min(tmax,max(tz1,tz2));
  }    
  return tmax>=tmin;
}
static int tri_hit(tri tr,struct ray r,vec3*i){
  const float SMALL_NUM=0.00000001;
  vec3 u,v,n;
  vec3 w0,w;
  float rt,a,b;
  u=vec3_sub(tr.q,tr.p);
  v=vec3_sub(tr.r,tr.p);
  n=vec3_cross(u,v);
  if((n.x=0)&&(n.y==0)&&(n.z==0)) return -1;
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
  if(NULL==t||!box_hit(t->bounds,r)) return 0;
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
