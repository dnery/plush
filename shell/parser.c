#include "parser.h"



char **Args(char *command, int *nargs){
  char **rt=(char **)malloc(MAX_ARGS*sizeof(char *));
  char *tmp=NULL;
  int i=0,flag=0;
  

  rt[i++]=strtok(command,TOK_DELIM );
  while((i<MAX_ARGS) && (rt[i++]=strtok(NULL,TOK_DELIM)));
  *nargs=(i-1); 
  
  return rt;  
}
