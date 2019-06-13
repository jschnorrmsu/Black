#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <assert.h>
#include <error.h>
// Contributors: Jake Schnorr, Luke Smith 

int main() {

    int exitStatus;
    // Create a child process, capture the child process ID on sucess.
    pid_t childPID = fork();

    if(childPID < 0) {
        perror("System call fork() failed\n");
        exit(-1);
    }

    else if(childPID == 0) {
        // Execute counter.c, capture program return status.
        exitStatus = execl("./counter", "counter", "5", NULL);
        perror("Function execl() failed\n");
        exit(exitStatus);
    }

    else {
        // Wait for child process to end, print exit status returned from counter.c upon sucess.
        assert(waitpid(childPID, &exitStatus, 0) != -1);
        if(WIFEXITED(exitStatus)) {
            assert(printf("Process exited with status: %d \n", WEXITSTATUS(exitStatus)) != 0);
        }

    }
    return 0;
}