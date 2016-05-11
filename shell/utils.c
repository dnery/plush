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

void set_free(char ***tgt, int args){
  int i;
  if(tgt==NULL || *tgt == NULL || args == 0) return;
  
  for(i=args-1;i>=0;i--) free(*tgt[i]);
  free(*tgt);
  tgt=NULL;
}
