#include "shell.h"
#include "parser.h"

#include <sys/types.h>
#include <sys/wait.h>

void shell_loop(){
  char *line=NULL;
  char **tgt=NULL;
  int exe_members, pid,status, execution,i; 
  
  while(1){
    printf(">> ");
    line=get_line();
    
    if(EmptyCmd(line)){
      tgt=Args(line,&exe_members);
      if(NullCmd(tgt[0])){
        
        KillCmd(tgt[0])
        
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
      
    }
    free(line);
    /*set_free(&tgt, exe_members);*/
  }
  printf("See ya\n");
}

void shell_init(){
  shell_loop();
}
