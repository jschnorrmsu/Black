#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>


/*
struct sigaction action;
action.sa_handler = handler;
sigemptyset (&action.sa_mask);
action.sa_flags = SA_RESTART;
assert (sigaction (SIGUSR1, &action, NULL) == 0 ); 
*/

void handler(int) {
    
    sigaction();

    //then wait for the child
    //waitpid();
    //and then when signals arrive, the signal handler begins doing its thing. 
    
}

int main() {

    // Create a child process, capture child PID# upon success. 
    pid_t childPID = fork();
    int exitStatus;

    if( childPID < 0) {
        perror("System call fork() failed\n");
        exit(-1);
    }

    else if( childPID == 0 ) {
        // Execute child.c, capture program return status. 
        exitStatus = execl("./child", "child", NULL);
        perror("Function execl() failed\n");
        exit(exitStatus);
    }

    else {
        assert(waitpid(childPID, &exitStatus, 0) != -1);
    }
    
    return 0;

} 
