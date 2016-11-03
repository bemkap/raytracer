#include"sdl.hh"

sdl::sdl(){
  SDL_Init(SDL_INIT_VIDEO);
  wi=SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
  re=SDL_CreateRenderer(wi,-1,SDL_RENDERER_ACCELERATED);
  te=SDL_CreateTexture(re,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,WIDTH,HEIGHT);
  SDL_QueryTexture(te,&fmt.format,nullptr,nullptr,nullptr);
  SDL_RenderClear(re);
}
sdl::~sdl(){
  SDL_DestroyTexture(te);
  SDL_DestroyRenderer(re);
  SDL_DestroyWindow(wi);
  SDL_Quit();
}
void sdl::draw(){
  SDL_UpdateTexture(te,nullptr,pixmap,WIDTH*4);
  SDL_RenderCopy(re,te,nullptr,nullptr);
  SDL_RenderPresent(re);
}
void sdl::set(int y,int x,glm::vec3 v){
  pixmap[(HEIGHT-y)*WIDTH+x]=SDL_MapRGB(&fmt,255*v.x,255*v.y,255*v.z);
}
