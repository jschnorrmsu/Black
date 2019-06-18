#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

void SIGUSR1Handler();
void SIGUSR2Handler();

int main() {

    printf("test output form main.c\n");
    int exitStatus;
    pid_t parentPID;
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