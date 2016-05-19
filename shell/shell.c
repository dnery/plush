#include "shell.h"
#include "parser.h"


void shell_loop(){
  char *line=NULL;
  char **tgt=NULL;
  int exe_members=0, pid,status,pos,i, *positions,fd_out; 
  
  while(1){
    printf(">> ");
    line=get_line();
    
    if(NotEmptyCmd(line)){
      tgt=Args(line,&exe_members);
      pos=Get_char_position(tgt,exe_members,'>');
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
          if(pos!=ERROR && pos!=NOT_FOUND){
            close(1);
            fd_out=open(tgt[pos+1], O_CREAT| O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

            dup2(fd_out,STDOUT_FILENO);

            tgt[pos]=tgt[pos+1]=NULL;
          }
          execvp(tgt[0],tgt);
          exit(EXIT_FAILURE);
        }
      }
      //set_free(&tgt,exe_members);
      exe_members=0;
       
    }
  }
  printf("See ya\n");
  
}

void shell_init(){
  shell_loop();
}
