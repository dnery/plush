#include "utils.h"


char *get_line(){
  char *rt=NULL; char chk='a'; int i=0; 
  do{
    chk=getchar();
    rt=(char *)realloc(rt,(i+1)*sizeof(char));
    rt[i++]=chk;

  }while(chk!='\n');
  rt[--i]='\0';
  return rt;
}


void set_free(char ***tgt,int args){
  int i=0;
  if(*tgt==NULL) return;
  for(i=0;i<args;i++) free(*(tgt[i]));
  free(*tgt);
}
