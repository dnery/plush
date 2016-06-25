#include "shell.h"
#include "debug.h"
#include "jobs.h"

#include <signal.h>

void sh_init()
{
        /* List head */
        first_job = NULL;

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

void sh_launch_job(job_t *job, int foreground)
{
        pid_t pid;                      /* PID for the newly spawned process */
        int output;                     /* Output file descriptor */
        int input = job->stdin;         /* Input file descriptor */
        int iopipe[2] = {-1,-1};        /* Inter-process comms pipe */

        process_t *p = job->first_process;
        for (; p != NULL; p = p->next) {

                /* Set up pipes if applicable, set output */
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
                        sh_launch_process(p, job->pgid, input, output,
                                        job->stderr, foreground);
                } else {
                        /* I am parent */
                        p->pid = pid;

                        if (shell_interactive) {

                                /* MAN SPECIFIES: If pgid is 0, pid of indicated process shall be used */
                                if (job->pgid == 0)
                                        job->pgid = pid;

                                setpgid(pid, job->pgid);
                        }
                }

                /* Clean up the pipes, set input */
                if (input != job->stdin)
                        close(input);
                if (output != job->stdout)
                        close(output);

                input = iopipe[0];
        }

        format_job_info(job, "launched");

        /* Either wait until job is finished, or engage new */
        if (!shell_interactive)
                job_wait_blocked(job);
        else if (foreground)
                put_in_foreground(job, 0);
        else
                put_in_background(job, 0);
}

void sh_launch_process(process_t *p, pid_t pgid, int in, int out, int err,
                int foreground)
{
        pid_t pid;

        if (shell_interactive) {
                pid = getpid();

                /* MAN SPECIFIES: If pgid is 0, pid of indicated process shall be used */
                if (pgid == 0)
                        pgid = pid;

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
         * Set stdanard IO channels for new process.
         *
         * TODO Redirection has to happen here.
         */
        if (in != STDIN_FILENO) {
                dup2(in, STDIN_FILENO);
                close(in);
        }
        if (out != STDOUT_FILENO) {
                dup2(out, STDOUT_FILENO);
                close(out);
        }
        if (err != STDERR_FILENO) {
                dup2(err, STDERR_FILENO);
                close(err);
        }

        /*
         * Exec and exit on error.
         */
        execvp(p->argv[0], p->argv);
        warn("End-point execvp.\n");
        exit(errno);
}

void sh_continue(job_t *job, int foreground)
{
        /* Mark suspended job as running again */
        process_t *p = job->first_process;
        for (; p != NULL; p = p->next)
                p->suspended = 0;
        job->notified = 0;

        /* Put in fg or bg, sending non-zero cont */
        if (foreground)
                put_in_foreground(job, 1);
        else
                put_in_background(job, 1);
}

void put_in_foreground(job_t *job, int cont)
{
        /* Put jab in foreground */
        tcsetpgrp(shell_terminal, job->pgid);

        /* Continue work, if specified */
        if (cont) {
                tcsetattr(shell_terminal, TCSADRAIN, &job->modes);
                check(kill(-job->pgid, SIGCONT) >= 0);
        }

        /* Wait for job to report */
        job_wait_blocked(job);

        /* Put shell back in command */
        tcsetpgrp(shell_terminal, shell_pgid);

        /* Restore shell terminal modes  */
        tcgetattr(shell_terminal, &job->modes);
        tcsetattr(shell_terminal, TCSADRAIN, &shell_modes);
}

void put_in_background(job_t *job, int cont)
{
        /* Just continue work, if so specified */
        if (cont)
                check(kill(-job->pgid, SIGCONT) >= 0);
}
