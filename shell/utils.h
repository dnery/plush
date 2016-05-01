#ifndef UTILS_H_
#define UTILS_H_

#define TOK_DELIM " \t\n"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define NullCmd(exp)  exp!=NULL ? 1 : 0
#define EmptyCmd(exp) strcmp(exp,"\0")!=0 ? 1 : 0

#define KillCmd(exp) if(!strcmp(exp,"quit")) break;

#define FATAL_ERROR(exp) do{if(exp){\
  fprintf(stderr," ERROR: %s : %d : %s \n",__FILE__, __LINE__,strerror(errno));\
  exit(EXIT_FAILURE);}}while(0);


#define FAILURE(exp, value) do{if(exp) {\
  fprintf(stderr, "Error: %s : %d : %s\n", __FILE__,__LINE__,strerror(errno));\
  exit(value);}}while(0);



char *get_line();


#endif
