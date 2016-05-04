#include "runcmd.h"
#include "debug.h"





int runcmd (const char *command, int *result, int *io){
  int pid=-1, status=0, kid,i=0,tmp_result=0, *bkp=NULL;
  char *args[RCMD_MAXARGS], *aux, *cmd;
  
  cmd=(char *)malloc(strlen(command+1)*sizeof(char));
  sysfail(!cmd, -1);
  aux=strcpy(cmd,command);

  args[i++]=strtok(cmd, RCMD_DELIM);
  while((i<RCMD_MAXARGS) && (args[i++]=strtok(NULL, RCMD_DELIM)));
  i--;

 
 
  if(!(CHKNULL(io))){
    bkp=(int *)malloc(3*sizeof(int));
    dup2(0,bkp[0]);
    dup2(1,bkp[1]);
    dup2(2, bkp[2]);
    close(0);
    close(1);
    close(2);
    dup2(io[0],0);
    dup2(io[1],1);
    dup2(io[2],2);
  }
 
  pid=fork();
  sysfail(pid<0, -1);
  
  
  
  /*non-blocking code
  if(*args[i]==RCMD_NONBLOCK){
  
  }
  */
  if(pid>0){ /*parent proccess*/
    kid=wait(&status);
    sysfail(kid<0,-1);
    if(io){
      close(0);
      close(1);
      close(2);
      dup2(bkp[0],0);
      dup2(bkp[1],1);
      dup2(bkp[2],2);
    }

    if(WIFEXITED(status)){
      tmp_result = (WEXITSTATUS(status) | NORMTERM);
      if(WEXITSTATUS(status)!=EXECFAILSTATUS) tmp_result |= EXECOK;
      else  tmp_result &= ~EXECOK;
    }
  }
  else{ /*child proccess*/
    kid=execvp(args[0],args);
    exit (EXECFAILSTATUS);
  }

   *result = tmp_result;
  free(aux);
  return pid;
}

void (*runcmd_onexit)(void) = NULL;
