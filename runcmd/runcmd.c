#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <runcmd.h>
#include <debug.h>





int runcmd (const char *command, int *result, int *io){
  int pid=-1, status=0, kid,i=0,tmp_result=0; /*bkp=NULL;*/
  char *args[RCMD_MAXARGS], *aux, *cmd;
  int exec_ok[2] ;
  cmd=(char *)malloc(strlen(command+1)*sizeof(char));
  sysfail(!cmd, -1);
  aux=strcpy(cmd,command);

  args[i++]=strtok(cmd, RCMD_DELIM);
  while((i<RCMD_MAXARGS) && (args[i++]=strtok(NULL, RCMD_DELIM)));
  i--;

 
 
  /* Redirection check.
   * if(!(CHKNULL(io))){*/
    /*bkp=(int *)malloc(3*sizeof(int));*/
    /*dup2(0,bkp[0]);*/
    /*dup2(1,bkp[1]);*/
    /*dup2(2, bkp[2]);*/
    /*close(0);*/
    /*close(1);*/
    /*close(2);*/
    /*dup2(io[0],0);*/
    /*dup2(io[1],1);*/
    /*dup2(io[2],2);*/
  /*}*/
  
  sysfail(pipe(exec_ok)<0,-1);

  pid=fork();
  sysfail(pid<0, -1);
  
  
  
  /*non-blocking code
  if(*args[i]==RCMD_NONBLOCK){
  
  }
  */
  if(pid>0){ /*parent proccess*/
    close(exec_ok[1]);
    kid=wait(&status);
    sysfail(kid<0,-1);
    /*if(io){*/
      /*close(0);*/
      /*close(1);*/
      /*close(2);*/
      /*dup2(bkp[0],0);*/
      /*dup2(bkp[1],1);*/
      /*dup2(bkp[2],2);*/
    /*}*/

    if(WIFEXITED(status)){
      tmp_result |=NORMTERM;
      tmp_result |=(WEXITSTATUS(status)&RETSTATUS);
      
      if(read(exec_ok[0],NULL,1)==0) 
        tmp_result |=EXECOK;
    }
  }
  else{ /*child proccess*/
    kid=execvp(args[0],args);
    write(exec_ok[1],"False!",1);
    close(exec_ok[1]);
    close(exec_ok[0]);
    exit (EXECFAILSTATUS);
  }

   *result = tmp_result;
  free(aux);
  return pid;
}

void (*runcmd_onexit)(void) = NULL;
