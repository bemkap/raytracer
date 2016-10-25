#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"parser.h"
#include"vec3.h"

struct container*container_new(unsigned sz){
  struct container*c=malloc(sizeof(struct container));
  c->ar=malloc(sz*sizeof(void*));
  c->av=0;
  c->sz=sz;
  return c;
}
unsigned container_push(struct container*c,void*n){
  if(c->av>=c->sz){c->ar=realloc(c->ar,c->sz*2*sizeof(void*)); c->sz*=2;}  
  c->ar[c->av++]=n;
  return c->av-1;
}
void container_destroy(struct container*c){
  for(long i=0; i<c->av; ++i) free(c->ar[i]);
  free(c->ar);
  free(c);
}
struct obj_desc*obj_parse(const char*fn){
  char buffer[128],cmd;
  FILE*in=fopen(fn,"r");
  float f[3]; int a[9];
  struct obj_desc*obj=malloc(sizeof(struct obj_desc));
  obj->vs=container_new(INITIAL_SIZE);
  obj->vts=container_new(INITIAL_SIZE);
  obj->ns=container_new(INITIAL_SIZE);
  obj->fs=container_new(INITIAL_SIZE);
  while(fgets(buffer,sizeof(buffer),in)){
    if(sscanf(buffer,"f %d/%d/%d %d/%d/%d %d/%d/%d",a+0,a+1,a+2, a+3,a+4,a+5, a+6,a+7,a+8)){
      face*f=malloc(sizeof(face));
      for(int i=0; i<9; ++i) f->a[i]=a[i];
      container_push(obj->fs,f);
    }else if(sscanf(buffer,"vt %f %f",f,f+1)){
      vec3*v=malloc(sizeof(vec3));
      v->x=f[0]; v->y=f[1];
      container_push(obj->vts,v);
    }else if(sscanf(buffer,"%c %f %f %f",&cmd,f,f+1,f+2)){
      vec3*v=malloc(sizeof(vec3));
      v->x=f[0]; v->y=f[1]; v->z=f[2];
      switch(cmd){
      case 'v': container_push(obj->vs,v); break;
      case 'n': container_push(obj->ns,v); break;
      default : free(v);
      }
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
