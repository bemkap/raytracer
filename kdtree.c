#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"kdtree.h"
#include"vec3.h"
#include"parser.h"

#define iface(o,i) ((face*)g_ptr_array_index((o)->fs,(i)))
#define ivertex(o,i,f) ((vec3*)g_ptr_array_index((o)->vs,iface(o,i)->f-1))
//#define inormal(o,i) ((vec3*)p_ptr_array_index((o)->ns,iface(o,i)
#define itriagle(o,i) (tri){{*ivertex(o,i,v0),*ivertex(o,i,v1),*ivertex(o,i,v2)}}
#define FCMP(f,c)                                                       \
  gint f(const void*a,const void*b,void*ex){                            \
    const face*fa=*(face**)a,*fb=*(face**)b;                            \
    GPtrArray*vs=ex;                                                    \
    float mpa=(                                                         \
               ((vec3*)g_ptr_array_index(vs,fa->v0))->c+                \
               ((vec3*)g_ptr_array_index(vs,fa->v1))->c+                \
               ((vec3*)g_ptr_array_index(vs,fa->v2))->c)/3;             \
    float mpb=(                                                         \
               ((vec3*)g_ptr_array_index(vs,fb->v0))->c+                \
               ((vec3*)g_ptr_array_index(vs,fb->v1))->c+                \
               ((vec3*)g_ptr_array_index(vs,fb->v2))->c)/3;             \
    return mpb-mpa;                                                     \
  }
FCMP(cx,x)
FCMP(cy,y)
FCMP(cz,z)
static kdtree*kdtree_new_aux(obj_desc*o,box b,unsigned d,long i,long j){
  kdtree*t=kdtree_leaf(i,j,d);
  t->bounds=b;
  if(j-i>25){
    GCompareDataFunc cmp[3]={cx,cy,cz};
    box bl,br; bl=br=b;
    g_qsort_with_data(o->fs->pdata+i,j-i+1,sizeof(void*),cmp[d%3],o->vs);
    bl.t.p[d%3]=ivertex(o,(i+j)/2,a[3*(d%3)])->p[d%3];
    t->left=kdtree_new_aux(o,bl,d+1,i,(i+j)/2);
    br.f.p[d%3]=ivertex(o,(i+j)/2,a[3*(d%3)])->p[d%3];
    t->right=kdtree_new_aux(o,br,d+1,(i+j)/2+1,j);
  }
  return t;
}
kdtree*kdtree_new(obj_desc*o,box b,unsigned sz){
  return kdtree_new_aux(o,b,0,0,sz-1);
}
kdtree*kdtree_leaf(long i,long j,unsigned d){
  kdtree*t=malloc(sizeof(kdtree));
  t->i=i; t->j=j;
  t->depth=d;
  t->left=t->right=NULL;
  return t;
}
static inline float max(float a,float b){ return a>b?a:b; }
static inline float min(float a,float b){ return a<b?a:b; }
int box_hit(box b,ray r){
  float tmin=-INFINITY,tmax=INFINITY;
  for(int i=0; i<3; ++i){
    if(r.d.p[i]!=0){
      float t1=(b.f.p[i]-r.p.p[i])/r.d.p[i];
      float t2=(b.t.p[i]-r.p.p[i])/r.d.p[i];
      tmin=max(tmin,min(t1,t2));
      tmax=min(tmax,max(t1,t2));
    }
  }
  return tmax>=tmin;
}
static int tri_hit(tri tr,ray r,vec3*i){
  vec3 u,v,n;
  vec3 w0,w;
  float rt,a,b;
  u=vec3_sub(tr.q,tr.p);
  v=vec3_sub(tr.r,tr.p);
  n=vec3_cross(u,v);
  if((0==n.x)&&(0==n.y)&&(0==n.z)) return 0;
  w0=vec3_sub(r.p,tr.p);
  a=-vec3_dot(n,w0);
  b=vec3_dot(n,r.d);
  if(fabs(b)<0.000001) return 0;
  rt=a/b;
  if(rt<0) return 0;
  *i=vec3_add(r.p,vec3_mul(rt,r.d));
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
int kdtree_hit(obj_desc*o,kdtree*t,ray r,vec3*v){
  if(NULL==t) return 0;
  else if((NULL==t->left)&&(NULL==t->right)){
    if(!box_hit(t->bounds,r)) return 0;
    for(long i=t->i; i<t->j; ++i){
      tri tr=itriagle(o,i);
      if(tri_hit(tr,r,v)) return 1;
    }; return 0;
  }else return kdtree_hit(o,t->left,r,v)||kdtree_hit(o,t->right,r,v);
} 
void kdtree_destroy(kdtree*t){
  if(NULL==t) return;
  kdtree_destroy(t->left);
  kdtree_destroy(t->right);
  free(t);
}
