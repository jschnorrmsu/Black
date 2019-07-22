#define _POSIX_C_SOURCE 200809l
#define errno (*__errno_location())
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <syscall.h>
#include <errno.h>
#include <time.h>
#include "syscall.h"
#include "eye2eh.c"

// With help from Luke Smith, John Shaprio. 

void handler();
pid_t childPID;

int main() {


    struct sigaction action;
    action.sa_handler = handler;
    assert (sigemptyset(&action.sa_mask) >= 0);
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    assert(sigaction(SIGCHLD, &action, NULL) ==0);

    childPID = fork();

    if( childPID < 0) {
        perror("system call fork() failed\n");
        exit(-1);
    }

    else if( childPID == 0) {
        execl("./child", "child", NULL);
        perror("execl() failure\n");
        exit(-1);
    }

    else {
        for(int i = 0; i < 5; i ++) {
            WRITESTRING("SIGSTOPPING child\n");
            assert(kill(childPID, SIGSTOP) == 0);
            assert(sleep(2) >= 0);
            WRITESTRING("SIGSTARTING child\n");
            assert(kill(childPID, SIGCONT) == 0);
            assert(sleep(2) >= 0);
        }
        assert(kill(childPID, SIGINT) == 0);
        assert(pause() == 0);
    }
    return(0);
}


void handler(int signum) {

    // SIGINT handler.
    if(signum == 2) {
        // Outside scope of this project.
    }

    // SIGCHLD handler.
    if(signum == 17) {
        int status = childPID;
        assert(waitpid(-1, &status, 0) != 0);

        if(WIFSIGNALED(status)) {
            int exitStatus = WTERMSIG(status);
            WRITESTRING("Child exited with status ");
            WRITEINT(status, sizeof(exitStatus));
            WRITESTRING("\n");
        }
    }

    // SIGCONT handler.
    if(signum == 18) {
        // Outside scope of this project.
    }

    // SIGSTOP handler.
    if(signum ==19) {
        // Outside scope of this project.
    }
    exit(0);
}