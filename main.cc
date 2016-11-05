#include"main.hh"
using namespace cfg;

int main(int argc,char*argv[]){
  if(argc<2) return 1;
  string fn(argv[1]);
  vec3 v,n;
  //sdl*g=new sdl();
  obj_desc*o=new obj_desc(fn);
  vector<long> ts(o->fs.size());
  generate_n(ts.begin(),o->fs.size(),[](){ static long n=0; return n++; });
  kdtree*t=new kdtree(o,b,0,ts);
  t->hit(o,r,v,n);
  cout<<v.x<<" "<<v.y<<" "<<v.z<<endl;
    /*
    for(int i=0; i<WIDTH; ++i)
    for(int j=0; j<HEIGHT; ++j){
    r.d=vec3(-9+0.06*i,-9+0.06*j,0)-r.o;
    if(t->hit(o,r,v,n)){
    vec3 m={0,0,0};
    for(size_t k=0; k<sizeof(l); ++k){
    vec3 nl=normalize(l[k].p-v);
    float d=dot(n,nl);
    if(d>0) m+=l[k].c*d*0.64f;
    }
    g->set(j,i,m);
    }
    }
    g->draw();
    int quit=0;
    SDL_Event e;
    while(!quit) while(SDL_PollEvent(&e)){
    switch(e.type){
    case SDL_QUIT: quit=1; break;
    }
    }
    delete g;
  */
  delete t;
  delete o;
  return 0;
}
