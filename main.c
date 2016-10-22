#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include"parser.h"
#include"kdtree.h"

SDL_Window*wi;
SDL_Renderer*re;
SDL_Texture*te;

static void kdtree_print(struct kdtree*t){
  if(NULL==t) return;
  for(int i=0; i<t->depth; ++i) printf("   ");
  printf("%d\n",t->depth);
  kdtree_print(t->left);
  kdtree_print(t->right);
}  
int main(int argc,char*argv[]){
  if(argc<2) return 1;
  Uint32 pixmap[300*300];

  SDL_Init(SDL_INIT_VIDEO);
  wi=SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,300,300,SDL_WINDOW_SHOWN);
  re=SDL_CreateRenderer(wi,-1,0);
  te=SDL_CreateTexture(re,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,300,300);
  
  struct obj_desc*o=obj_parse(argv[1]);
  tri tris[o->fs->av];
  for(int i=0; i<o->fs->av; ++i){
    tris[i].p=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[0]-1];
    tris[i].q=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[3]-1];
    tris[i].r=*(vec3*)o->vs->ar[((int*)o->fs->ar[i])[6]-1];
  }
  struct kdtree*t=kdtree_new(tris,o->fs->av);
  struct ray r;
  vec3 v;
  r.p0=(vec3){0.0,-100.0,0.0};
  for(int i=0; i<300; ++i){
    for(int j=0; j<300; ++j){
      r.dir=vec3_sub((vec3){(float)i/10-15,0,(float)j/10-15},r.p0);
      pixmap[i*300+j]=0xffffff*(kdtree_hit(t,r,&v));
    }
  }
  SDL_UpdateTexture(te,NULL,pixmap,300*4);
  SDL_RenderCopy(re,te,NULL,NULL);
  SDL_RenderPresent(re);
  SDL_Delay(2000);
  kdtree_destroy(t);
  obj_destroy(o);

  SDL_DestroyTexture(te);
  SDL_DestroyRenderer(re);
  SDL_DestroyWindow(wi);
  SDL_Quit();
  
  return 0;
}
