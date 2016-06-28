#include "shell.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
        job_t *job;
        char **pargv;

        sh_init();

        pargv = &(argv[1]);

        job = create_job(shell_pgid);
        create_process(job, pargv);
        sh_launch_job(job, 1);
        job_do_notify();

        return 0;
}
