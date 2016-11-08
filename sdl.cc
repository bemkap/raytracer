#define GLM_FORCE_RADIANS
#include<glm/ext.hpp>
#include"sdl.hh"

sdl::sdl():r(double(WIDTH)/double(HEIGHT)){
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
void sdl::clear(){
  SDL_SetRenderDrawColor(re,0,0,0,0);
  SDL_RenderClear(re);
}
void sdl::draw(){
  SDL_RenderPresent(re);
}
void sdl::set(int y,int x,glm::dvec3 v){
  SDL_SetRenderDrawColor(re,255*v.x,255*v.y,255*v.z,255);
  SDL_RenderDrawPoint(re,x,HEIGHT-y);
}
void sdl::wait_input(dvec3&p,bool&quit,bool&redraw){
  SDL_Event e;
  while(SDL_PollEvent(&e)){
    switch(e.type){
    case SDL_QUIT: quit=true; return;
    case SDL_KEYDOWN: switch(e.key.keysym.sym){
      case SDLK_1: p.x-=1.5; break;
      case SDLK_2: p.x+=1.5; break;
      case SDLK_3: p.y-=1.5; break;
      case SDLK_4: p.y+=1.5; break;
      case SDLK_5: p.z-=1.5; break;
      case SDLK_6: p.z+=1.5; break;
      }
    }
  }
  quit=false; redraw=true;
}
void sdl::r2s(double i,double j,double&x,double&y,double fov){
  x=(2*((i+0.5)/double(WIDTH))-1)*tan(radians(fov/2))*r;
  y=(1-2*((j+0.5)/double(HEIGHT)))*tan(radians(fov/2));
}
