#define _POSIX_C_SOURCE 200809l
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <syscall.h>

void handler();

int main() {

    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    //call pause at the end of main so that the arent doesnt 
    // process diesnt exit from there. 

}

void handler(int signum) {

    pid_t parentPID = getppid();
    // SIGCHLD handler.
    if(signum == 17) {
        for(int i = 0; i <= 4; i ++) {
            write(1, "Parent with ID# %d is going to SIGSTOP the child.", parentPID);
            //SIGSTOP the child
            sleep(2);

            write(1, "Parent with ID# %d is going to SIGCONT the child", parentPID);
            //SIGCONT the child
            sleep(2);
        }

    }

    // SIGINT handler. 
    if(signum == 2) {
        //handle SIGINT here. 
    }

}