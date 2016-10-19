#include"kdtree.h"
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

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
unsigned kdtree_count(struct kdtree*t){
  return NULL==t ? 0 : 1+kdtree_count(t->left)+kdtree_count(t->right);
}
void kdtree_destroy(struct kdtree*t){
  if(NULL==t) return;
  kdtree_destroy(t->left);
  kdtree_destroy(t->right);
  free(t);
}
