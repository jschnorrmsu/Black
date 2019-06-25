#define _POSIX_C_SOURCE 200809l
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    pid_t parentPID = getppid();
    pid_t childPID = getpid();
    //assert(kill(parentPID, SIGCHLD) != -1);
    while(1) {
        printf("Awake in %d\n", childPID);
    }

    return(0);
}