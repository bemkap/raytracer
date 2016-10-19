#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"parser.h"

struct container*container_new(unsigned sz){
  struct container*c=malloc(sizeof(struct container));
  c->ar=malloc(sz*sizeof(void*));
  c->av=0;
  c->sz=sz;
  return c;
}
unsigned container_push(struct container*c,void*n){
  c->ar[c->av]=n;
  if(++c->av>=c->sz) c->ar=realloc(c->ar,c->sz*2);
  return c->av-1;
}
void container_destroy(struct container*c){
  for(long i=0; i<c->sz; ++i) free(c->ar[i]);
  free(c->ar);
  free(c);
}
struct obj_desc*obj_parse(const char*f){
  char buffer[128],cmd[2];
  FILE*in=fopen(f,"r");
  int i[9];
  struct obj_desc*obj=malloc(sizeof(struct obj_desc));
  obj->vs=container_new(INITIAL_SIZE);
  obj->vts=container_new(INITIAL_SIZE);
  obj->ns=container_new(INITIAL_SIZE);
  obj->fs=container_new(INITIAL_SIZE);
  while(fgets(buffer,sizeof(buffer),in)){
    if(sscanf("%s %f %f %f",buffer,cmd,&f[0],&f[1],&f[2])){
      vec3*v=malloc(sizeof(vec3));
      v->x=f[0]; v->y=f[1]; v->z=f[2];
      if(0==strcmp("v",cmd)) container_push(obj->vs,v);
      else if(0==strcmp("vt",cmd)) container_push(obj->vts,v);
      else if(0==strcmp("n",cmd)) container_push(obj->ns,v);
    }else if(sscanf("f %d/%d/%d %d/%d/%d %d/%d/%d",buffer,&i[0],&i[1],&i[2], &i[3],&i[4],&i[5], &i[6],&i[7],&i[8])){
      int*v=malloc(9*sizeof(int));
      v[0]=i[0]; v[1]=i[1]; v[2]=i[2];  v[3]=i[3]; v[4]=i[4]; v[5]=i[5];  v[6]=i[6]; v[7]=i[7]; v[8]=i[8];
      container_push(obj->fs,v);
    }
  }
  fclose(in);
  return obj;
}
void obj_destroy(struct obj_desc*obj){
  container_destroy(obj->vs);
  container_destroy(obj->vts);
  container_destroy(obj->ns);
  container_destroy(obj->fs);
  free(obj);
}
