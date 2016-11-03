#include<cmath>
#include<algorithm>
#include<glm/glm.hpp>
#include"kdtree.hh"
#include"prim.hh"
#include"parser.hh"
using namespace std;

kdtree::kdtree(obj_desc*o,box b,unsigned d,long i,long j):i(i),j(j),depth(d),bounds(b),left(nullptr),right(nullptr){
  if(j-i>25&&d<20){
    box bl,br; bl=br=b;
    auto cmp=[o,d](face fa,face fb){
      float mpa=std::min(std::min(o->vs[fa.v0-1][d%3],o->vs[fa.v1-1][d%3]),o->vs[fa.v2-1][d%3]);
      float mpb=std::min(std::min(o->vs[fb.v0-1][d%3],o->vs[fb.v1-1][d%3]),o->vs[fb.v2-1][d%3]);
      return mpa<mpb;
    };
    sort(o->fs.begin()+i,o->fs.begin()+j+1,cmp);
    bl.t[d%3]=std::min(std::min(o->f2v((i+j)/2,0)[d%3],
                                o->f2v((i+j)/2,1)[d%3]),
                       o->f2v((i+2)/2,2)[d%3]);
    br.f[d%3]=std::min(std::min(o->f2v((i+j)/2+1,0)[d%3],
                                o->f2v((i+j)/2+1,1)[d%3]),
                       o->f2v((i+2)/2+1,2)[d%3]);
    left =new kdtree(o,bl,d+1,i,(i+j)/2);
    right=new kdtree(o,br,d+1,(i+j)/2+1,j);
  }
}
kdtree::~kdtree(){
  if(nullptr!= left) delete  left;
  if(nullptr!=right) delete right;
}
bool kdtree::hit(obj_desc*o,ray r,vec3&v,vec3&n){
  if(!r.hit(bounds)) return false;
  else if((nullptr==left)&&(nullptr==right)){
    for(long a=i; a<j; ++a){
      triangle tr={o->vs[o->fs[a].v0-1],
		   o->vs[o->fs[a].v1-1],
		   o->vs[o->fs[a].v2-1]};
      vec3 uv;
      if(r.hit(tr,v,uv)){
        vec3 n0=o->ns[o->fs[a].n0-1];
        vec3 n1=o->ns[o->fs[a].n1-1];
        vec3 n2=o->ns[o->fs[a].n2-1];
        n=uv.z*n0+(1-uv.y-uv.z)*n1+uv.y*n2;
        return true;
      }
    }; return false;
  }else{
    bool le=left?left->hit(o,r,v,n):false;
    bool ri=right?right->hit(o,r,v,n):false;
    return le||ri;
  }
}

