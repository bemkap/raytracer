#include"parser.h"
#include"kdtree.h"

void main(int argc,char*argv[]){
  if(argc<2) return;
  struct obj_desc*obj=obj_parse(argv[1]);
  obj_destroy(obj);
}
