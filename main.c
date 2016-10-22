#include<stdio.h>
#include<stdlib.h>
#include"parser.h"
#include"kdtree.h"

static void kdtree_print(struct kdtree*t){
  if(NULL==t) return;
  for(int i=0; i<t->depth; ++i) printf("   ");
  printf("%d\n",t->depth);
  kdtree_print(t->left);
  kdtree_print(t->right);
}  
int main(int argc,char*argv[]){
  if(argc<2) return 1;
  struct obj_desc*o=obj_parse(argv[1]);
  tri tris[o->fs->av];
  for(int i=0; i<o->fs->av; ++i){
    tris[i].p=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[0]-1];
    tris[i].q=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[3]-1];
    tris[i].r=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[6]-1];
  }
  struct kdtree*t=kdtree_new(tris,o->fs->av);
  struct ray r={{0.446948,4.60535,-6.06405},{-0.25739,-0.62013,0.741073}};
  vec3 v;
  printf("%d\n",kdtree_hit(t,r,&v));
  kdtree_destroy(t);
  obj_destroy(o);
  return 0;
}
