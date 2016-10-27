#include<stdio.h>
#include<stdlib.h>
#include<SDL.h>
#include"parser.h"
#include"kdtree.h"
#include"time.h"


int main(int argc,char*argv[]){
  if(argc<2) return 1;
  SDL_Window*wi;
  SDL_Renderer*re;
  SDL_Texture*te;
  Uint32 pixmap[300*300];
  SDL_Init(SDL_INIT_VIDEO);
  wi=SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,300,300,SDL_WINDOW_SHOWN);
  re=SDL_CreateRenderer(wi,-1,0);
  te=SDL_CreateTexture(re,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,300,300);
  obj_desc*o=obj_parse(argv[1]);
  box b={{{-10,-10,-10}},{{10,10,10}}};
  vec3 l=vec3_normal((vec3){{-10,-10,-10}});
  kdtree*t=kdtree_new(o,b,o->fs->len);
  ray r; vec3 v;
  r.p=(vec3){{0,3,-30}};
  for(int i=0; i<300; ++i){
    for(int j=0; j<300; ++j){
      r.d=(vec3){{(float)(i-150)*0.001,(float)(j-150)*0.001,0.5}};
      if(kdtree_hit(o,t,r,&v)) pixmap[i*300+j]=0xffffff*vec3_dot(vec3_normal(v),l);
      else pixmap[i*300+j]=0;
    }
  }
  SDL_UpdateTexture(te,NULL,pixmap,300*4);
  SDL_RenderCopy(re,te,NULL,NULL);
  SDL_RenderPresent(re);
  SDL_Delay(5000);
  SDL_DestroyTexture(te);
  SDL_DestroyRenderer(re);
  SDL_DestroyWindow(wi);
  SDL_Quit();
  kdtree_destroy(t);
  obj_destroy(o);
}

// Rayo 1: 
//  r.p=(vec3){{0,3,-30}};                      
//  r.d=(vec3){{0.00105257,0.00883711,0.99996}};
// Punto de impacto: 0.0264568	3.22213	-4.86548	
// 
// Rayo 2: 
//  r.p=(vec3){{19.4733,3.01545,16.592}};
//  r.d=(vec3){{-0.754998,0.00343363,-0.655718}};
// Punto de impacto: 3.85954	3.08646	3.03135	
// 
// Rayo 3: 
//  r.p=(vec3){{8.05917,-3.68279,2.01399}}
//  r.d=(vec3){{-0.620148,0.714088,-0.3248}}
// Punto de impacto: 4.17958	0.784479	-0.0179293	
// 
// Rayo 4:   
//  r.p=(vec3){{-0.0039109,3.38799,0.408742}}	
//  r.d=(vec3){{0.299255,0.696669,0.651996}}
// Punto de impacto: 1.16409	6.10711	2.95351	
// 
// Rayo 5: 
//  r.p=(vec3){{-2.73964,18.5392,3.71494}}
//  r.d=(vec3){{0.222011,-0.919783,-0.323589}}
// Punto de impacto: -0.16381	7.86759	-0.0394237
