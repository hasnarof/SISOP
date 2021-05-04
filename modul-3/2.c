#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <wait.h>
#include <stdlib.h>

int main() 
{ 
    if (chdir("/home/meizee/Documents/git_env/SISOP/modul-2") < 0) {
        exit(EXIT_FAILURE);
    }

    int fd[2], status1, status2;
    char* argv1[] = {"ls", NULL, NULL};
    char* argv2[] = {"head", "-3", NULL};
    char* argv3[] = {"tail", "-1", NULL};
    

    if (pipe(fd) < 0)
        exit(1);

    if (fork() == 0) {
        dup2(fd[1], 1); 
        execv("/usr/bin/ls", argv1);
    }

    if (fork() == 0){
        dup2(fd[0], 0); 
        dup2(fd[1], 1);
        execv("/usr/bin/head", argv2);
    }

    if (fork() == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        execv("/usr/bin/tail", argv3);
    }
    
    close(fd[0]);
    close(fd[1]);

    wait(NULL); 
    wait(NULL);
    wait(NULL);

    exit(1);
    return 0; 
} 