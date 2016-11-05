#include"main.hh"
using namespace cfg;

int main(int argc,char*argv[]){
  if(argc<2) return 1;
  string fn(argv[1]);
  vec3 v,n; aabb b;
  // sdl*g=new sdl();
  obj*o=new obj(fn);
  vector<long> ts(o->fs.size());
  generate_n(ts.begin(),o->fs.size(),[](){ static long n=0; return n++; });
  for(auto v:o->vs)
    for(int i=0; i<3; ++i){
      b.f[i]=std::min(b.f[i],v[i]);
      b.t[i]=std::max(b.t[i],v[i]);
    }
  kdtree*t=new kdtree(o,b,0,ts);
  // for(auto i=0u; i<o->fs.size(); ++i){
  //   triangle tr=o->f2t(i);
  //   if(r.hit(tr,v,n)){cout<<i<<endl; break;}
  // }
  // cout<<v.x<<' '<<v.y<<' '<<v.z<<endl;
  if(t->hit(o,r,v,n))
    cout<<v.x<<' '<<v.y<<' '<<v.z<<endl;
  // for(int i=0; i<WIDTH; ++i)
  //   for(int j=0; j<HEIGHT; ++j){
  //     r.d=vec3(-9+0.06*i,-9+0.06*j,0)-r.o;
  //     if(t->hit(o,r,v,n)){
  // 	vec3 m={0,0,0};
  // 	for(size_t k=0; k<sizeof(l); ++k){
  // 	  vec3 nl=normalize(l[k].p-v);
  // 	  float d=dot(n,nl);
  // 	  if(d>0) m+=l[k].c*d*0.64f;
  // 	}
  // 	g->set(j,i,m);
  //     }
  //   }
  // g->draw();
  // int quit=0;
  // SDL_Event e;
  // while(!quit) while(SDL_PollEvent(&e)){
  //     switch(e.type){
  //     case SDL_QUIT: quit=1; break;
  //     }
  //   }
  // delete g;
  delete t;
  delete o;
  return 0;
}
