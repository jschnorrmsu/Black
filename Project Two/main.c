#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>

void SIGUSR1Handler();
void SIGUSR2Handler();

int main() {

    int exitStatus;
    pid_t parentPID;

    // Set handlers for SIGUSR1 and SIGUSR2 signals.
    signal (SIGUSR1, SIGUSR1Handler);
    signal (SIGUSR2, SIGUSR2Handler);

    struct sigaction action;
    action.sa_handler = SIGUSR1Handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;
    //action.sa_flags = SA_RESTART;

    assert (sigaction (SIGUSR1, &action, NULL) == 0 );

    // Create a child process, capture child PID# upon success. 
    pid_t childPID = fork();

    if( childPID < 0) {
        perror("System call fork() failed\n");
        exit(-1);
    }

    else if( childPID == 0 ) {

        parentPID = getppid();
        //Execute child.c, capture program return status. 
        exitStatus = execl("./child", "child", NULL);
        perror("Function execl() failed\n");
        exit(exitStatus);
    }

    else {

        assert(waitpid(childPID, &exitStatus, 0) != -1);
    }
    
    return 0;
} 

void SIGUSR1Handler(int signal) {
    
    //sigaction();
    printf("test output from SIGUSR1Hanlder\n");
    //then wait for the child
    //waitpid();
    //and then when signals arrive, the signal handler begins doing its thing.   
}

void SIGUSR2Handler(int signal) {

    printf("test output from SIGUSR2Handler\n");
}