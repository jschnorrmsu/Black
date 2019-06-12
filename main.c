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
    pid_t childPID = fork(); 

    if(childPID < 0) {
        perror("System call <fork()> failed\n");
        exit(-1);
    }

    else if(childPID == 0) {
        exitStatus = execl("./counter", "counter", "5", NULL);
    }

    else {
        assert(waitpid(childPID, &exitStatus, 0) != -1);
    }
    return 0;
}