#include<stdio.h>
#include<stdlib.h>
#include<SDL.h>
#include"parser.h"
#include"kdtree.h"
#include"time.h"

enum {HEIGHT=300,WIDTH=300};

int main(int argc,char*argv[]){
  /* if(argc<2) return 1; */
  /* SDL_Window*wi; */
  /* SDL_Renderer*re; */
  /* SDL_Texture*te; */
  /* Uint32 pixmap[HEIGHT*WIDTH]={0}; */
  /* SDL_Init(SDL_INIT_VIDEO); */
  /* wi=SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN); */
  /* re=SDL_CreateRenderer(wi,-1,0); */
  /* te=SDL_CreateTexture(re,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,WIDTH,HEIGHT); */
  box b={{{-10,-10,-10}},{{10,10,10}}}; ray r; vec3 v,n,l=(vec3){{6,4,9}},nl=vec3_normal(l);
  obj_desc*o=obj_parse(argv[1]);
  kdtree*t=kdtree_new(o,b,o->fs->len);
  r.p=(vec3){{-2.73964,18.5392,3.71494}};//(vec3){{0,3,-30}};
  r.d=(vec3){{0.222011,-0.919783,-0.323589}};
  if(kdtree_hit(o,t,r,&v,&n)) vec3_print(v);
  /* for(int i=0; i<WIDTH; ++i){ */
  /*   for(int j=0; j<HEIGHT; ++j){ */
  /*     r.d=(vec3){{i-WIDTH/2.0,j-HEIGHT/2.0,500}}; */
  /*     if(kdtree_hit(o,t,r,&v,&n)){ */
  /*       float m=255*fabs(vec3_dot(n,nl)); */
  /*       pixmap[(HEIGHT-j)*WIDTH+i]=SDL_MapRGB(SDL_GetWindowSurface(wi)->format,m,m,m); */
  /*     } */
  /*   } */
  /* } */
  /* SDL_UpdateTexture(te,NULL,pixmap,WIDTH*4); */
  /* SDL_RenderCopy(re,te,NULL,NULL); */
  /* SDL_RenderPresent(re); */
  /* SDL_Delay(5000); */
  /* SDL_DestroyTexture(te); */
  /* SDL_DestroyRenderer(re); */
  /* SDL_DestroyWindow(wi); */
  /* SDL_Quit(); */
  kdtree_destroy(t);
  obj_destroy(o);
}

/* Punto de impacto: -0.16381	7.86759	-0.0394237	 */
