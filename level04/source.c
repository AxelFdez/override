#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>

int clear_stdin() {
    char c;
    do {
        c = getchar();  
        if (c == '\n') break;
    } while (c != EOF);
    return c;
}

int get_unum() {
    int result = 0;
    fflush(stdout);
    scanf("%d", &result);
    clear_stdin();
    return result;
}

void prog_timeout() {
    _exit(1);
}

void enable_timeout_cons() {
    signal(SIGALRM, prog_timeout);
    alarm(60);
}

int main(int argc, char** argv, char** envp) {
    pid_t pid = fork();
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    int stat_loc = 0;

    if (pid != 0) { // processus parent
        while (1) {
            wait(&stat_loc); // bloque le processus parent jusqu'à ce que l'état du processus enfant change (par exemple, l'enfant se termine ou est stoppé par un signal).

            if (WIFEXITED(stat_loc) || WIFSIGNALED(stat_loc)) { // vérifie si l'enfant a terminé normalement, et vérifie si l'enfant a été terminé par un signal. Si l'une de ces conditions est vraie, cela imprime un message et sort de la boucle.
                puts("child is exiting...");
                break;
            }

            if (ptrace(PTRACE_PEEKUSER, pid, 44, 0) != 0xb) {
                continue;
            }

            puts("no exec() for you");
            kill(pid, SIGKILL); // Si le processus enfant tente d'exécuter exec(), il imprime un message et envoie un signal SIGKILL à l'enfant pour le terminer immédiatement, puis sort de la boucle.
            break;
        }
    } else {
        prctl(PR_SET_DUMPABLE, 1);
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        puts("Give me some shellcode, k");
        gets(buffer);
    }

    return 0;
}
