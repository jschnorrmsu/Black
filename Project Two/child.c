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
    assert (kill(parentPID, SIGUSR1) != -1);
    assert (kill(parentPID, SIGUSR2) != -1);
    assert (kill(parentPID, SIGINT) != -1);

    assert (kill(parentPID, SIGINT) != -1);
    assert (kill(parentPID, SIGINT) != -1);
    assert (kill(parentPID, SIGINT) != -1);

    return(0);
}
