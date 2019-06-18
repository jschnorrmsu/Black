#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <stdio.h>

// Child sends signals to parent --> parent will contain the signla 
// handlers (all of them) and have them ALL SET UP BEFORE the fork() 
// and execl() to child.) send to:

int main() {
    int returnValue;
    int parentPID = getppid();

/*
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = SA_RESTART;
    assert (sigaction (SIGUSR1, &action, NULL) == 0 ); 
*/

        // Set handlers for SIGUSR1 and SIGUSR2 signals.
        signal (SIGUSR1, SIGUSR1Handler);
        signal (SIGUSR2, SIGUSR2Handler);

        kill(parentPID, SIGUSR1);
        kill(parentPID, SIGUSR2);

    return returnValue;
}
