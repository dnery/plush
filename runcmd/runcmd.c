/* runcmd.c - Library developed during OS2 course. 
Copyright (c) 2014, <Henrique Freitas> - <freitash1792@gmail.com>

This file is part of <Henrique Freitas>

RunCmd is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

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
  int pid=-1, status=0, kid,i=0,tmp_result=0, *bkp=NULL;
  char *args[RCMD_MAXARGS], *aux, *cmd;
  int exec_ok[2];


  cmd=(char *)malloc(strlen(command+1)*sizeof(char));
  sysfail(!cmd, -1);
  aux=strcpy(cmd,command);

  args[i++]=strtok(cmd, RCMD_DELIM);
  while((i<RCMD_MAXARGS) && (args[i++]=strtok(NULL, RCMD_DELIM)));
  i--;

 
 
  if(!(CHKNULL(io))){
   bkp=(int *)malloc(3*sizeof(int));
   memcpy(&bkp[0],stdin,sizeof(int));
   memcpy(&bkp[1],stdout,sizeof(int));
   memcpy(&bkp[2],stderr,sizeof(int));
   close(0);
   close(1);
   close(2);
   dup2(io[0],0);
   dup2(io[1],1);
   dup2(io[2],2);
  }
  
  sysfail(pipe(exec_ok)<0,-1);
  pid=fork();
  sysfail(pid<0, -1);
  
  
  
  if(pid>0){ /*parent proccess*/
    close(exec_ok[1]);
    if(*args[i]==RCMD_NONBLOCK){ 
      return 0;
    }
    
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
      tmp_result |= NORMTERM;
      tmp_result |= (WEXITSTATUS(status)&RETSTATUS);
      
      if(read(exec_ok[0],NULL,1)==0) 
        tmp_result |=EXECOK;
    }
  }
  else{ /*child proccess*/
    if(*args[i]==RCMD_NONBLOCK){
      kid=execvp(args[0],args);
      raise(SIGKILL);
    }
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
