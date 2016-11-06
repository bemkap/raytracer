#include"sdl.hh"

sdl::sdl(){
  SDL_Init(SDL_INIT_VIDEO);
  wi=SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
  re=SDL_CreateRenderer(wi,-1,SDL_RENDERER_ACCELERATED);
  SDL_RenderClear(re);
}
sdl::~sdl(){
  SDL_DestroyRenderer(re);
  SDL_DestroyWindow(wi);
  SDL_Quit();
}
void sdl::draw(){
  SDL_RenderPresent(re);
}
void sdl::set(int y,int x,glm::vec3 v){
  SDL_SetRenderDrawColor(re,255*abs(v.x),255*abs(v.y),255*abs(v.z),255);
  SDL_RenderDrawPoint(re,x,HEIGHT-y);
}
void sdl::waitexit(){
  int quit=0;
  SDL_Event e;
  while(!quit) while(SDL_PollEvent(&e)){
      switch(e.type){
      case SDL_QUIT: quit=1; break;
      }
    }
}
