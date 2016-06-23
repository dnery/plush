#include "shell.h"
#include "debug.h"

#include <signal.h>

void sh_init()
{
        /* Check for interactivity */
        shell_terminal = STDIN_FILENO;
        shell_interactive = isatty(shell_terminal);

        if (shell_interactive) {
                /* ACTIVE BLOCK UNTIL WE ARE FOREGROUND */
                while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
                        kill(-shell_pgid, SIGTTIN);

                /* IGNORE PERTINENT SIGNALS */
                signal(SIGINT, SIG_IGN);
                signal(SIGQUIT, SIG_IGN);
                signal(SIGTSTP, SIG_IGN);
                signal(SIGTTIN, SIG_IGN);
                signal(SIGTTOU, SIG_IGN);
                signal(SIGCHLD, SIG_IGN);

                /* INSERT SELF INTO GROUP */
                shell_pgid = getpid();
                check(setpgid(shell_pgid, shell_pgid) >=0);

                /* Take control */
                tcsetpgrp(shell_terminal, shell_pgid);

                /* Save attributes */
                tcgetattr(shell_terminal, &shell_modes);

                info(":3\n");
        }
}

void sh_launchj(job_t *job, int foreground)
{
        pid_t pid;                      /* PID for the newly spawned process */
        int input;                      /* Input file descriptor */
        int output;                     /* Output file descriptor */
        int iopipe[2] = {-1,-1};        /* Inter-process comms pipe */

        input = job->stdin;

        process_t *p = job->first_proc;
        for (; p != NULL; p = p->next) {

                /* Set up pipes if applicable and output */
                if (p->next != NULL) {
                        check(pipe(iopipe) >= 0);
                        output = iopipe[1];
                } else {
                        output = job->stdout;
                }

                /* Fork child process and launch */
                check((pid = fork()) >= 0);

                if (pid == 0) {
                        /* I am the fork */
                        sh_launchp(p, job->pgid, input, output, job->stderr,
                                        foreground);
                } else {
                        /* I am parent */
                        p->pid = pid;

                        if (shell_interactive) {

                                /* MAN SPECIFIES: If pgid is 0, pid of indicated process shall be used */
                                /* if (job->pgid == 0)
                                        job->pgid = pid; */

                                setpgid(pid, job->pgid);
                        }
                }

                /* Clean up the pipes */
                if (input != job->stdin)
                        close(input);
                if (output != job->stdout)
                        close(output);

                /* Set up input */
                input = iopipe[0];
        }
        /* Job is launched */
        job_format_info(job, "launched");

        /* Either wait until job is finished */
        if (!shell_interactive)
                job_wait_blocked(job);
        /* Otherwise you're ready to engage a new job */
        else if (foreground)
                put_in_foreground(job, 0);
        else
                put_in_background(job, 0);
}

void sh_launchp(process_t *p, pid_t pgid, int in, int out, int err,
                int foreground)
{
        pid_t pid;

        if (shell_interactive) {
                pid = getpid();

                /* MAN SPECIFIES: If pgid is 0, pid of indicated process shall be used */
                /* if (pgid == 0)
                        pgid = pid; */

                setpgid(pid, pgid);

                if (foreground)
                        tcsetpgrp(shell_terminal, pgid);

                /* Restore default signal handling */
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                signal(SIGTTIN, SIG_DFL);
                signal(SIGTTOU, SIG_DFL);
                signal(SIGCHLD, SIG_DFL);
        }

        /*
         * TODO A lotta shit here!
         */
}
