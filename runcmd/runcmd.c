#include <runcmd.h>
#include <debug.h>





int runcmd (const char *command, int *result, int *io){
  int pid=-1, status=0, kid,i=0,tmp_result=0, *fd;
  char *args[RCMD_MAXARGS], *aux, *cmd;
  
  cmd=(char *)malloc(strlen(command+1)*sizeof(char));
  sysfail(!cmd, -1);
  aux=strcpy(cmd,command);

  args[i++]=strtok(cmd, RCMD_DELIM);
  while((i<RCMD_MAXARGS) && (args[i++]=strtok(NULL, RCMD_DELIM)));
  i--;

 
 
  if(!(CHKNULL(io))){
    close(0);
    close(1);
    close(2);
    dup2(io[0],0);
    dup2(io[1],1);
    dup2(io[2],2);
  }
 
  pid=fork();
  sysfail(pid<0, -1);
  
  
  
  //non-blocking code. 
  if(*args[i]==RCMD_NONBLOCK){
  
  }

  if(pid>0){ //parent proccess
    kid=wait(&status);
    sysfail(aux<0,-1);

    if(WIFEXITED(status)){ 
      tmp_result = (WEXITSTATUS(status) | NORMTERM);
      printf("WEXITSTATUS %d \n", WEXITSTATUS(status));
    }
  }else{ //child proccess
    kid=execvp(args[0],args);
    exit(EXECFAILSTATUS);
  }

  if(result) *result = tmp_result;
  free(aux);
  free(fd);
  return pid;
}

void (*runcmd_onexit)(void) = NULL;
