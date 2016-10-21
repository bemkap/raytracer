#include<stdio.h>
#include<stdlib.h>
#include"parser.h"
#include"kdtree.h"

int main(int argc,char*argv[]){
  if(argc<2) return 1;
  struct obj_desc*o=obj_parse(argv[1]);
  tri tris[o->fs->av];
  for(int i=0; i<o->fs->av; ++i){
    tris[i].p=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[0]];
    tris[i].q=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[3]];
    tris[i].r=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[6]];
  }
  struct kdtree*t=kdtree_new(tris,o->fs->av);
  kdtree_destroy(t);
  obj_destroy(o);
  return 0;
}
