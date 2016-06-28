#include "shell.h"
#include "input.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
        command_t *cmd;

        sh_init();

        while ((cmd = read_command()) != NULL) {

                job_t *job = create_job(shell_pgid);
                create_process(job, cmd->argv);
                sh_launch_job(job, cmd->fg);

                job_do_notify();
                delete_command(cmd);
        }

        return 0;
}
