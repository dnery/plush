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

int Get_char_position(char **Args, int Nargs, char tgt){
  int i;
  if(Args==NULL || *Args == NULL || Nargs<=0) return ERROR;
  
  for(i=0;i<Nargs;i++){
    if(*Args[i]==tgt){
      return i;
    }
  }

  if(*Args[i]==tgt)   return i;
  else  return NOT_FOUND;
}


char **mv_redir(char ***Args, int size){
  char **rt=NULL;  
  int i;
  if(*Args==NULL||**Args==NULL||size<=0) return NULL;
  
  rt=(char**)malloc((size-2)*sizeof(char*));

  for(i=0;i<(size-2);i++){
    rt[i]=(char *)malloc((sizeof(**Args[i]))*sizeof(char));
    memcpy(rt[i],*Args[i],sizeof(**Args[i]));
  }
  return rt;
}
