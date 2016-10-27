#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"parser.h"
#include"vec3.h"

obj_desc*obj_parse(const char*fn){
  char buffer[128],cmd;
  FILE*in=fopen(fn,"r");
  float f[3]; int a[9];
  obj_desc*obj=malloc(sizeof(obj_desc));
  obj->vs =g_ptr_array_new();
  obj->vts=g_ptr_array_new();
  obj->ns =g_ptr_array_new();
  obj->fs =g_ptr_array_new();
  while(fgets(buffer,sizeof(buffer),in)){
    if(sscanf(buffer,"f %d/%d/%d %d/%d/%d %d/%d/%d",
              a+0,a+1,a+2, a+3,a+4,a+5, a+6,a+7,a+8)){
      face*f=malloc(sizeof(face));
      for(int i=0; i<9; ++i) f->a[i]=a[i];
      g_ptr_array_add(obj->fs,f);
    }else if(sscanf(buffer,"vt %f %f",f,f+1)){
      vec3*v=malloc(sizeof(vec3));
      v->x=f[0]; v->y=f[1];
      g_ptr_array_add(obj->vts,v);
    }else if(sscanf(buffer,"%c %f %f %f",&cmd,f,f+1,f+2)){
      vec3*v=malloc(sizeof(vec3));
      v->x=f[0]; v->y=f[1]; v->z=f[2];
      switch(cmd){
      case 'v': g_ptr_array_add(obj->vs,v); break;
      case 'n': g_ptr_array_add(obj->ns,v); break;
      default : free(v);
      }
    }
  }
  fclose(in);
  return obj;
}
void obj_destroy(obj_desc*obj){
  g_ptr_array_free(obj->vs,1);
  g_ptr_array_free(obj->vts,1);
  g_ptr_array_free(obj->ns,1);
  g_ptr_array_free(obj->fs,1);
  free(obj);
}
