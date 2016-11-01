#include"main.hh"

enum {HEIGHT=300,WIDTH=300};

int main(int argc,char*argv[]){
  if(argc<2) return 1;
  string ofn(argv[1]);
  /* SDL_Window*wi; */
  /* SDL_Renderer*re; */
  /* SDL_Texture*te; */
  /* SDL_Init(SDL_INIT_VIDEO); */
  /* wi=SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN); */
  /* re=SDL_CreateRenderer(wi,-1,0); */
  /* te=SDL_CreateTexture(re,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,WIDTH,HEIGHT); */
  box b={{-10,-10,-10},{10,10,10}}; //ray r;
  /* light l[]={{{{0,3,-30}},{{0,255,0}}},{{{7,12,-10}},{{0,0,255}}}}; */
  /* vec3 v,n; */
  /* int quit=0; */
  /* SDL_Event e; */
  obj_desc*o=new obj_desc(ofn);
  kdtree*t=new kdtree(o,b);
  /* Uint32 pixmap[HEIGHT*WIDTH]; */
  /* r.p=(vec3){{0,3,-30}}; */
  /* SDL_RenderClear(re); */
  /* for(int i=0; i<WIDTH; ++i) */
  /*   for(int j=0; j<HEIGHT; ++j){ */
  /*     r.d=vec3_sub((vec3){{-9+0.06*i,-9+0.06*j,0}},r.p); */
  /*     if(kdtree_hit(o,t,r,&v,&n)){ */
  /*       vec3 m={{0,0,0}}; */
  /*       for(int k=0; k<2; ++k){ */
  /*         vec3 nl=vec3_sub(l[k].p,v); */
  /*         float d=vec3_dot(n,nl); */
  /*         if(d>0) m=vec3_add(m,vec3_mul(d*0.64,l[k].c)); */
  /*         pixmap[(HEIGHT-j)*WIDTH+i]=SDL_MapRGB(SDL_GetWindowSurface(wi)->format,m.x*255,m.y*255,m.z*255); */
  /*       } */
  /*     } */
  /*   } */
  /* SDL_UpdateTexture(te,NULL,pixmap,WIDTH*4); */
  /* SDL_RenderCopy(re,te,NULL,NULL); */
  /* SDL_RenderPresent(re); */
  /* while(!quit) while(SDL_PollEvent(&e)){ */
  /*     switch(e.type){ */
  /*     case SDL_QUIT: quit=1; break; */
  /*     } */
  /*   } */
  /* SDL_DestroyTexture(te); */
  /* SDL_DestroyRenderer(re); */
  /* SDL_DestroyWindow(wi); */
  /* SDL_Quit(); */
  delete t; delete o;
  return 0;
}
