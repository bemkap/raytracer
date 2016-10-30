#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<SDL.h>
#include"parser.h"
#include"kdtree.h"
#include"time.h"

#define M_PI 3.14159265358979323846
enum {HEIGHT=300,WIDTH=300};

int main(int argc,char*argv[]){
  if(argc<2) return 1;
  SDL_Window*wi;
  SDL_Renderer*re;
  SDL_Init(SDL_INIT_VIDEO);
  wi=SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
  re=SDL_CreateRenderer(wi,-1,0);
  box b={{{-10,-10,-10}},{{10,10,10}}}; ray r; vec3 v,n,l=(vec3){{6,4,9}},nl=vec3_normal(l);
  int quit=0,redraw=1;
  SDL_Event e;
  obj_desc*o=obj_parse(argv[1]);
  kdtree*t=kdtree_new(o,b,o->fs->len);
  r.p=(vec3){{0,3,-30}};
  while(!quit){if(redraw){
      redraw=0;
      SDL_SetRenderDrawColor(re,0,0,0,0);
      SDL_RenderClear(re);
      SDL_RenderPresent(re);
      for(int i=0; i<WIDTH; ++i)
	for(int j=0; j<HEIGHT; ++j){
	  r.d=(vec3){{i-WIDTH/2.0,j-HEIGHT/2.0,500}};
	  if(kdtree_hit(o,t,r,&v,&n)){
	    float m=255*fabs(vec3_dot(n,nl));
	    SDL_SetRenderDrawColor(re,m,m,m,255);
	    SDL_RenderDrawPoint(re,i,HEIGHT-j);
	    SDL_RenderPresent(re);
	  }
	}
    }
    while(SDL_PollEvent(&e)){
      switch(e.type){
      case SDL_QUIT: quit=1; break;
      }
    }
  }
  SDL_DestroyRenderer(re);
  SDL_DestroyWindow(wi);
  SDL_Quit();
  kdtree_destroy(t);
  obj_destroy(o);
}
