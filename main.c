#include<stdio.h>
#include<stdlib.h>
#include<SDL.h>
#include"parser.h"
#include"kdtree.h"
#include"time.h"

//#define GRAPH

int main(int argc,char*argv[]){
  if(argc<2) return 1;
#ifdef GRAPH
  SDL_Window*wi;
  SDL_Renderer*re;
  SDL_Texture*te;
  Uint32 pixmap[300*300];
  SDL_Init(SDL_INIT_VIDEO);
  wi=SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,300,300,SDL_WINDOW_SHOWN);
  re=SDL_CreateRenderer(wi,-1,0);
  te=SDL_CreateTexture(re,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,300,300);
#endif
  struct obj_desc*o=obj_parse(argv[1]);
  struct kdtree*t=kdtree_new(o,o->fs->av);
  struct ray r; vec3 v;
  r.p0=(vec3){0,3,-30};
  r.dir=(vec3){0.00105257,0.00883711,0.99996};
  /* Punto de impacto: 0.0264568 3.22213 -4.86548 */
  if(kdtree_hit(t,r,&v)) vec3_print(v);
#ifdef GRAPH
  for(int i=0; i<300; ++i){
    for(int j=0; j<300; ++j){
      r.dir=(vec3){(float)i/20-15,1,(float)j/20-15};
      pixmap[i*300+j]=0xffffff*kdtree_hit(t,r,&v);
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
#endif
  kdtree_destroy(t);
  obj_destroy(o);
}
