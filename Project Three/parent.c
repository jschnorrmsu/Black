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
#include <string.h>
#include "syscall.h"
#include "eye2eh.c"

void handler();

int main() {

    pid_t childPID;
    int exitStatus;

    struct sigaction action;
    action.sa_handler = handler;
    assert (sigemptyset(&action.sa_mask) >= 0);
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    childPID = fork();

    assert(sigaction(SIGCHLD, &action, NULL) ==0);

    if( childPID < 0) {
        perror("system call fork() failed\n");
        exit(-1);
    }  

    else if( childPID == 1) {
        exitStatus = execl("./child.c", "child", NULL);
        //***********FIXME add perror************************************************************
        //**********************************
    }   

    else {
        for(int i = 0; i < 5; i ++) {
            WRITESTRING("SIGSTOP child\n");
            assert(kill(exitStatus, SIGSTOP) == 0);
            sleep(2);
            WRITESTRING("SIGSTART child\n");
            assert(kill(exitStatus, SIGCONT) == 0);
            sleep(2);
        }
        assert(kill(exitStatus, SIGINT) == 0);
        pause();
    }
    return(0);
}


void handler(int signum) {

    // SIGINT handler.
    if(signum == 2) {
        //handle SIGINT here.
    }

    // SIGCHLD handler.
    if(signum == 17) {
        for(int i = 0; i <= 4; i ++) {
            int status = status;
            assert(waitpid(-1, &status, 0) != 0);

            if(WIFSIGNALED(status)) {
                int exitStatus = WTERMSIG(status);
                WRITESTRING("Child exited with status ");
                WRITEINT(status, 2);
                WRITESTRING("\n");
            }
        }

        exit(0);
    }


    // SIGCONT handler.
    if(signum == 18) {

    }

    // SIGSTOP handler.
    if(signum ==19) {

    }

}