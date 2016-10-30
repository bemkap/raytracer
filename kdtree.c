#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"kdtree.h"
#include"vec3.h"
#include"parser.h"

static inline float max(float a,float b){return a>b?a:b;}
static inline float min(float a,float b){return a<b?a:b;}
gint fcmp(const void*a,const void*b,void*ex,unsigned d){
  const face*fa=*(face**)a,*fb=*(face**)b;
  GPtrArray*vs=ex;
  float mpa=
    min(((vec3*)g_ptr_array_index(vs,fa->v0-1))->p[d%3],
	min(((vec3*)g_ptr_array_index(vs,fa->v1-1))->p[d%3],
	    ((vec3*)g_ptr_array_index(vs,fa->v2-1))->p[d%3]));
  float mpb=
    min(((vec3*)g_ptr_array_index(vs,fb->v0-1))->p[d%3],
	min(((vec3*)g_ptr_array_index(vs,fb->v1-1))->p[d%3],
	    ((vec3*)g_ptr_array_index(vs,fb->v2-1))->p[d%3]));
  return mpa-mpb;
}
gint cx(const void*a,const void*b,void*ex){return fcmp(a,b,ex,0);}
gint cy(const void*a,const void*b,void*ex){return fcmp(a,b,ex,1);}
gint cz(const void*a,const void*b,void*ex){return fcmp(a,b,ex,2);}
static kdtree*kdtree_new_aux(obj_desc*o,box b,unsigned d,long i,long j){
  kdtree*t=kdtree_leaf(i,j,d);
  t->bounds=b;
  if(j-i>25){
    GCompareDataFunc cmp[3]={cx,cy,cz};
    box bl,br; bl=br=b;
    g_qsort_with_data(o->fs->pdata+i,j-i+1,sizeof(void*),cmp[d%3],o->vs);
    bl.t.p[d%3]=ivertex(o,(i+j)/2,d)->p[d%3];
    t->left=kdtree_new_aux(o,bl,d+1,i,(i+j)/2);
    br.f.p[d%3]=ivertex(o,(i+j)/2,d)->p[d%3];
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
static int box_hit(box b,ray r){
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
static int tri_hit(tri tr,ray r,vec3*i,vec3*c){
  vec3 u,v,n,w0,w;
  float rt,a,b;
  float uu,uv,vv,wu,wv,D;
  float s,t;
  //plane
  u=vec3_sub(tr.q,tr.p); v=vec3_sub(tr.r,tr.p);
  n=vec3_cross(u,v);
  if((0==n.x)&&(0==n.y)&&(0==n.z)) return 0;
  w0=vec3_sub(r.p,tr.p);
  a=-vec3_dot(n,w0); b=vec3_dot(n,r.d);
  if(fabs(b)<0.001) return 0;
  rt=a/b;
  if(rt<0) return 0;
  *i=vec3_add(r.p,vec3_mul(rt,r.d));
  //triangle
  uu=vec3_dot(u,u); vv=vec3_dot(v,v); uv=vec3_dot(u,v);
  w=vec3_sub(*i,tr.p); wu=vec3_dot(w,u); wv=vec3_dot(w,v);
  D=uv*uv-uu*vv;
  s=(uv*wv-vv*wu)/D;
  if(s<0.0||s>1.0) return 0;
  t=(uv*wu-uu*wv)/D;
  if(t<0.0||(s+t)>1.0) return 0;
  *c=(vec3){{0,s,t}};//barycentric coordinates
  return 1;
}
int kdtree_hit(obj_desc*o,kdtree*t,ray r,vec3*v,vec3*n){
  if(NULL==t) return 0;
  else if((NULL==t->left)&&(NULL==t->right)){
    if(!box_hit(t->bounds,r)) return 0;
    for(long i=t->i; i<t->j; ++i){
      tri tr=itriangle(o,i); vec3 uv;
      if(tri_hit(tr,r,v,&uv)){
        vec3*n0=inormal(o,i,0),*n1=inormal(o,i,1),*n2=inormal(o,i,2);
        *n=vec3_add(vec3_add(vec3_mul(uv.z,*n0),
                             vec3_mul(1-uv.y-uv.z,*n1)),
                    vec3_mul(uv.y,*n2));
        return 1;
      }
    }; return 0;
  }else return kdtree_hit(o,t->left,r,v,n)||kdtree_hit(o,t->right,r,v,n);
} 
void kdtree_destroy(kdtree*t){
  if(NULL==t) return;
  kdtree_destroy(t->left);
  kdtree_destroy(t->right);
  free(t);
}
