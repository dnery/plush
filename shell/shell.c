#include "shell.h"
#include "parser.h"

#include <sys/types.h>
#include <sys/wait.h>

void shell_loop(){
  char *line=NULL;
  char **tgt=NULL;
  int exe_members=0, pid,status, execution,i; 
  
  while(1){
    printf(">> ");
    line=get_line();
    
    if(NotEmptyCmd(line)){
      tgt=Args(line,&exe_members);
      if(NotNullCmd(tgt[0])){
        
        if(!strcmp(tgt[0],"exit")){
          free(tgt);
          free(line);
          break;
        }
        pid=fork();
        FATAL_ERROR(pid<0);
      
        if(pid>0){
          wait(&status);
        }
        else{
          execvp(tgt[0],tgt);
          exit(EXIT_FAILURE);
        }
      }
      set_free(&tgt,exe_members);
      exe_members=0;
       
    }
  }
  printf("See ya\n");
  
}

void shell_init(){
  shell_loop();
}
