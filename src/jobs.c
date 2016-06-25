#include "jobs.h"
#include "debug.h"

#include <sys/wait.h>

job_t *find_job(pid_t pgid)
{
        job_t *j = first_job;
        for (; j != NULL; j = j->next)
                if (j->pgid == pgid)
                        return j;

        return NULL;
}

int is_suspended(job_t *job)
{
        process_t *p = job->first_process;
        for (; p != NULL; p = p->next)
                if (!p->suspended)
                        return 0;

        return 1;
}

int is_completed(job_t *job)
{
        process_t *p = job->first_process;
        for (; p != NULL; p = p->next)
                if (!p->completed)
                        return 0;

        return 1;
}

int update_status(pid_t pid, int status)
{
        if (pid > 0) {

                job_t *j = first_job;
                for (; j != NULL; j = j->next) {

                        process_t *p = j->first_process;
                        for (; p != NULL; p = p->next) {

                                if (p->pid == pid) {
                                        p->status = status;

                                        if (WIFSTOPPED(status)) {
                                                p->suspended = 1;
                                        } else {
                                                p->completed = 1;

                                                if (WIFSIGNALED(status))
                                                        fprintf(stderr, "Process %d terminated by sygnal %d.\n", (int)pid, WTERMSIG(p->status));
                                        }

                                        return 0;
                                }
                        }
                }
                /* BAAAD */
                warn("Worst case return...\n");
                return -1;
        } else if (pid == 0 || errno == ECHILD) {
                /* BAAAD */
                warn("No child processes.\n");
                return -1;
        } else {
                /* BAAAD */
                warn("Weird waipid exit.\n");
                return -1;
        }
}

/* ========================================================================== */

void job_update_status()
{
        int status;     /* Status retrieved from waitpid */
        pid_t pid;      /* PID retrieved from waitpid */

        do {
                pid = waitpid(-1, &status, WUNTRACED|WNOHANG);
        } while (!update_status(pid, status));
}

void job_wait_blocked(job_t *job)
{
        int status;     /* Status retrieved from waitpid */
        pid_t pid;      /* PID retrieved from waitpid */

        do {
                pid = waitpid(-1, &status, WUNTRACED);
        } while (!update_status(pid, status)
                        && !is_suspended(job)
                        && !is_completed(job));
}

void job_do_notify()
{
        job_t *j, *nextj, *lastj; /* Job iterators */

        job_update_status();

        lastj = NULL;
        j = first_job;
        for (; j != NULL; j = nextj) {
                nextj = j->next;

                if (is_completed(j)) {
                        /* Upon completion, notify user & delete job */
                        format_job_info(j, "completed");

                        if (lastj)
                                lastj->next = nextj;
                        else
                                first_job = nextj;

                        delete_job(j);
                } else if (is_suspended(j) && !j->notified) {
                        /* Otherwise, notify user and mark jobs */
                        format_job_info(j, "suspended");
                        j->notified = 1;
                        lastj = j;
                } else {
                        /* Silently process running jobs */
                        lastj = j;
                }
        }
}
