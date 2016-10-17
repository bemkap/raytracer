#include"kdtree.h"
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

typedef int(*fcmp)(const void*,const void*);

#define FCMP(f,c)                                    \
  int f(const void*a,const void*b){                  \
    return ((vec3*)a)->c-((vec3*)b)->c;              \
  }

FCMP(cx,x); FCMP(cy,y); FCMP(cz,z);

static struct kdtree*kdtree_new_aux(vec3*ns,unsigned d,unsigned i,unsigned j){
  if(i>=j) return NULL;
  fcmp cmp[3]={cx,cy,cz};
  qsort(ns+i,j-i,sizeof(vec3),cmp[d%3]);
  struct kdtree*t=kdtree_leaf(ns[(i+j-1)/2],d);
  t->left=kdtree_new_aux(ns,d+1,i,(i+j-1)/2);
  t->right=kdtree_new_aux(ns,d+1,(i+j-1)/2+1,j);
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
//ns tiene que tener suficiente espacio
static unsigned kdtree_collect(vec3*ns,struct kdtree*t){
  if(NULL==t) return 0;
  *ns=t->node;
  unsigned i=kdtree_collect(ns+1,t->left);
  return i+kdtree_collect(ns+i,t->right);
}  
struct kdtree*kdtree_remove(struct kdtree*t,vec3 n){
  if(NULL==t) return NULL;
  if(0<ondepth(n,t->node,t->depth)) t->right=kdtree_remove(t->right,n);
  else if(0>ondepth(n,t->node,t->depth)) t->left=kdtree_remove(t->left,n);
  else{
    unsigned sz=kdtree_count(t);
    vec3*ns=malloc(sz*sizeof(vec3));
    kdtree_collect(ns,t);
    t->node.x=INFINITY; t->node.y=INFINITY; t->node.z=INFINITY;
    struct kdtree*s=kdtree_new_aux(ns,t->depth,0,sz);
    kdtree_destroy(t);
    free(ns);
    return s;
  }
  return t;
}
struct kdtree*kdtree_nearest(struct kdtree*t,vec3 n){
  if(NULL==t) return NULL;
  if(t->node.x==INFINITY&&t->node.y==INFINITY&&t->node.z==INFINITY)
    {kdtree_destroy(t); return NULL;}
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
void kdtree_print(struct kdtree*t){
  if(NULL==t) return;
  for(int i=0; i<t->depth; ++i) printf("             ");
  printf("├─(%2.0f,%2.0f,%2.0f)─┤",t->node.x,t->node.y,t->node.y);
  puts("");
  kdtree_print(t->right);
  kdtree_print(t->left);
}
