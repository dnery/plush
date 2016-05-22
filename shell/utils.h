#ifndef UTILS_H_
#define UTILS_H_

#define MAX_ARGS 1024
#define TOK_DELIM " \t\n"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NotNullCmd(exp)  exp!=NULL ? 1 : 0
#define NotEmptyCmd(exp) strcmp(exp,"\0")!=0 ? 1 : 0
#define KillCmd(exp) if(!strcmp(exp,"exit")) break;

#define NOT_FOUND 404
#define ERROR 127

#define FATAL_ERROR(exp) do{if(exp){\
  fprintf(stderr," ERROR: %s : %d : %s \n",__FILE__, __LINE__,strerror(errno));\
  exit(EXIT_FAILURE);}}while(0);


#define FAILURE(exp, value) do{if(exp) {\
  fprintf(stderr, "Error: %s : %d : %s\n", __FILE__,__LINE__,strerror(errno));\
  exit(value);}}while(0);



char *get_line();
void set_free(char ***tgt,int args);
int Get_char_position(char **Args, int Nargs, char tgt);
char **mv_redir(char ***Args, int size);

#endif
