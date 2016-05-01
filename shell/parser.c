#include "parser.h"



char **Args(char *command, int *nargs){
  char **rt=NULL;
  char *tmp=NULL;
  int i=0,flag=0;
  
  tmp=strtok(command,TOK_DELIM);
  if(tmp==NULL) flag=1;
  while(!flag){
    rt=(char **)realloc(rt,(i+1)*sizeof(char *));
    rt[i]=(char *)malloc(sizeof(tmp));
    strcpy(rt[i++],tmp);
    tmp=strtok(NULL,TOK_DELIM);
    if(tmp==NULL) flag=1;
  }
 
 *nargs=i; 
  return rt;  
}
