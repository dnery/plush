#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "sup.h"

int startRoutines(int args, char **argv){
    return 0;
}

int shellLoop(){
    int pid = 0;
    int status = 0;
    BUF_T *tgt = NewCmd();

    while(status = 0){ //Loop de comandos
        read_cmd(tgt);
        //ADICIONAR>>> Compara se cmd é exit//
        pid = fork(); //Cria um processo novo

        if(pid > 0){ //Loop do processo pai
            wait();
        }
        else{
            execvp(); //Executa comando
            exit(EXIT_FAILURE); //Trata falhas de execução e deadlock
        }
    }

    return status;
}

int exitRoutines(){
    return 0;
}
