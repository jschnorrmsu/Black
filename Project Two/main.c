#define _POSIX_C_SOURCE 200809l
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <syscall.h>

void handler();

int main() {

    int exitStatus;

    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = SA_RESTART;

    assert (sigaction (SIGUSR1, &action, NULL) == 0 );
    assert (sigaction (SIGUSR2, &action, NULL) == 0 );
    assert (sigaction (SIGINT, &action, NULL) == 0);

    pid_t childPID = fork();

    if( childPID < 0) {
        perror("System call fork() failed\n");
        exit(-1);
    }

    else if( childPID == 0 ) { 
        exitStatus = execl("./child", "child", NULL);
        perror("Function execl() failed\n");
        exit(exitStatus);
    }

    else {
        assert(waitpid(childPID, &exitStatus, 0) != -1);
    }
    
    return 0;
} 

void handler(int signum) { 

    if(signum == 2) {
        assert(printf("Output from signal# %d, SIGINT\n", signum) != 0);
    } 

    if(signum == 10) {
        assert(printf("Output from signal# %d, SIGUSR1\n", signum) != 0);
    }

    if(signum == 12) {
        assert(printf("Output from signal# %d, SIGUSR2\n", signum) != 0);
    }
}



