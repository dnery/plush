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
