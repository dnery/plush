#include "jobs.h"
#include "debug.h"

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

int set_process_status(pid_t pid, int status)
{
        /* BAAAD */
        if (pid == 0 || errno == ECHILD) {
                warn("No child processes.\n");
                return -1;
        }

        /* BAAAD */
        if (pid < 0) {
                warn("I don't even...\n");
                return -1;
        }

        job_t *j = first_job;
        for (; j != NULL; j = j->next) {

                process_t *p = j->first_process;
                for (; p != NULL; p = p->next) {

                        if (p->pid == pid) {
                                /*
                                 * TODO
                                 */
                        }
                }
        }

        return 0;
}

/* ========================================================================== */

void job_update_status();

void job_wait_blocked(job_t *job);

void job_format_info(job_t *job, const char *status);

void job_do_notify();
