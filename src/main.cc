#include<algorithm>
#include<functional>
#include<iostream>
#include<pthread.h>
#include<random>
#include"kdtree.hh"
#include"obj.hh"
#include"prim.hh"
#include"setup.hh"

constexpr int WIDTH=640,HEIGHT=480,NTHREAD=1;
constexpr double RATIO=double(WIDTH)/double(HEIGHT);
constexpr double SAMPLES=1,FOV=90.0;

struct thread_setup {setup*s; int start; ray r;};

void*fill(void*p){
  thread_setup*ts=(thread_setup*)p;
  setup*s=ts->s;
  double x,y,z=tan(radians(FOV*0.5));
  for(int i=ts->start; i<HEIGHT*WIDTH*3; i+=NTHREAD*3){
    dvec3 I(0,0,0),v;
    for(int k=0; k<SAMPLES; ++k){
      x=(2*((double(i/3%WIDTH))/double(WIDTH))-1)*z*RATIO;
      y=(1-2*((double(i/3/WIDTH))/double(HEIGHT)))*z;
      ts->r.direct(x,y);
      s->t->hit(s->o,ts->r,I,v,s->ls,0);
    }
    I*=1.0/SAMPLES; saturate(I);
    s->scr[i+0]=(unsigned char)(I.x);
    s->scr[i+1]=(unsigned char)(I.y);
    s->scr[i+2]=(unsigned char)(I.z);
  }
  pthread_exit(NULL);
}
int main(int argc,char*argv[]){
  if(argc<1) return 1;
  setup s(string(argv[1]),WIDTH,HEIGHT);
  pthread_t threads[NTHREAD];
  thread_setup thread_setups[NTHREAD];
  pthread_attr_t attr;
  void*status;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
  
  for(int i=0; i<NTHREAD; ++i){
    thread_setups[i]={&s,i*3,s.r};
    pthread_create(&threads[i],&attr,fill,(void*)&thread_setups[i]);
  }
  pthread_attr_destroy(&attr);
  for(int i=0; i<NTHREAD; ++i)
    pthread_join(threads[i],&status);

  s.out<<"P6 "<<WIDTH<<' '<<HEIGHT<<" 255"<<endl;
  for(size_t i=0; i<HEIGHT*WIDTH*3; ++i) s.out<<s.scr[i];
  pthread_exit(0);
}
