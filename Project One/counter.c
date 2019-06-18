#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
// Contributors: Jake Schnorr, Luke Smith. 

int main(int argc, char*argv[]) {

    char* endPointer;
    pid_t childPID = getpid();
    int parentPID = getppid();
    long int countToThis = strtol(argv[1], &endPointer, 10);
    assert(errno == 0);

    assert(printf("Child PID: %d \nParent PID: %d\n", childPID, parentPID) != 0);

    if(argc <= 1) {
        assert(printf("Only one argument passed, arguments should be in form" 
        " <file name>, <char array[]>") != 0);
        exit(EXIT_FAILURE);
    }
    
    for(int i = 1; i <= countToThis; i ++) {
        assert(printf("Process ID:  %d %d \n", childPID, i) != 0);
    
    }

    return countToThis;
}