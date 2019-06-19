#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

// Child sends signals to parent --> parent will contain the signal 
// handlers (all of them) and have them ALL SET UP BEFORE the fork() 
// and execl() to child.)

int main() {

    pid_t parentPID = getppid();
    assert (kill(parentPID, SIGUSR1) != -1);
    assert (kill(parentPID, SIGUSR2) != -1);
    printf("test output from child.c\n");
    return(0);
}
