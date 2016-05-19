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

int *Get_char_position(char **Args, int Nargs){
  int i, *value=NULL,j=0;
  if(Args==NULL || *Args == NULL || Nargs<=0) return NULL;
  
  for(i=0;i<Nargs;i++){
    if(*Args[i]=='|' || *Args[i]=='>'){
     value=(int *)realloc(value,(j+1)*sizeof(int));
     value[j++]=i;
    }
  }
  if(value==NULL){
    value=(int *)malloc(2*sizeof(int));
    value[0]=NOT_FOUND;
    value[1]='\0';
  }
  else  value[j]='\0';
  return value;
}
