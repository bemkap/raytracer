#include"kdtree.h"

#include<stdlib.h>

typedef int(*fcmp)(const void*,const void*);

#define FCMP(f,c)                                    \
  int f(const void*a,const void*b){                  \
    return ((vec3*)a)->c-((vec3*)b)->c;              \
  }

FCMP(cx,x); FCMP(cy,y); FCMP(cz,z);

static struct kdtree*kdtree_new_aux(vec3*ns,unsigned d,unsigned i,unsigned j){
  fcmp cmp[3]={cx,cy,cz};
  qsort(ns+i,j-i,sizeof(vec3),cmp[d%3]);
  struct kdtree*t=kdtree_leaf(ns[(i+j)/2],d);
  t->left=kdtree_new_aux(ns,d+1,i,(i+j)/2);
  t->right=kdtree_new_aux(ns,d+1,(i+j)/2+1,j);
  return t;
}
struct kdtree*kdtree_new(vec3*ns,unsigned sz){
  return kdtree_new_aux(ns,0,0,sz);
}
struct kdtree*kdtree_leaf(vec3 v,unsigned d){
  struct kdtree*t=malloc(sizeof(struct kdtree));
  t->node=v;
  t->depth=d;
  t->left=t->right=NULL;
  return t;
}
static inline int ondepth(const vec3 a,const vec3 b,unsigned d){
  return a.p[d%3]-b.p[d%3];
}  
struct kdtree*kdtree_add(struct kdtree*t,vec3 n){
  if(NULL==t) return kdtree_leaf(n,0);
  if(0<ondepth(n,t->node,t->depth)) t->right=kdtree_add(t->right,n);
  else t->left=kdtree_add(t->left,n);
  t->right->depth=t->left->depth=t->depth+1;
  return t;
}
struct kdtree*kdtree_remove(struct kdtree*t,vec3 n){
  if(NULL==t) return NULL;
  if(0<ondepth(n,t->node,t->depth)) t->right=kdtree_remove(t->right,n);
  else if(0>ondepth(n,t->node,t->depth)) t->left=kdtree_remove(t->left,n);
  return t;
}
struct kdtree*kdtree_nearest(struct kdtree*t,vec3 n){
  if(NULL==t) return NULL;
  if(0<ondepth(n,t->node,t->depth)) return kdtree_nearest(t->right,n);
  else if(0>ondepth(n,t->node,t->depth)) return kdtree_nearest(t->left,n);
  return t;
}
