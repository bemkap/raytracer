#include<stdio.h>
#include<stdlib.h>
#include<SDL.h>
#include"parser.h"
#include"kdtree.h"
#include"time.h"


int main(int argc,char*argv[]){
  if(argc<2) return 1;
  obj_desc*o=obj_parse(argv[1]);
  box b={(vec3){{-10,-10,-10}},{{10,10,10}}};
  kdtree*t=kdtree_new(o,b,o->fs->len);
  ray r; vec3 v;
  r.p=(vec3){{19.4733,3.01545,16.592}};
  r.d=(vec3){{-0.754998,0.00343363,-0.655718}};
  kdtree_hit(o,t,r,&v); vec3_print(v);
  kdtree_destroy(t);
  obj_destroy(o);
}

// Rayo 1: 
//  {{0,3,-30}};                      
//  {{0.00105257,0.00883711,0.99996}};
// Punto de impacto: 0.0264568	3.22213	-4.86548	
// 
// Rayo 2: 
//  
//  
// Punto de impacto: 3.85954	3.08646	3.03135	
// 
// Rayo 3: 
//  {{8.05917,-3.68279,2.01399}}
//  {{-0.620148,0.714088,-0.3248}}
// Punto de impacto: 4.17958	0.784479	-0.0179293	
// 
// Rayo 4:   
//  {{-0.0039109,3.38799,0.408742}}	
//  {{0.299255,0.696669,0.651996}}
// Punto de impacto: 1.16409	6.10711	2.95351	
// 
// Rayo 5: 
//  {{-2.73964,18.5392,3.71494}}
//  {{0.222011,-0.919783,-0.323589}}
// Punto de impacto: -0.16381	7.86759	-0.0394237
