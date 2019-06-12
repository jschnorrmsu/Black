#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
// Contributors: Jake Schnorr, Luke Smith. 

int main(int argc, char*argv[]) {

    char* endPointer;
    int childPID = getpid();
    int parentPID = getppid();
    long int countToThis = strtol(argv[1], &endPointer, 10);

    // Parent/Child PID should always return > 0 
    assert(parentPID > 0);
    assert(childPID > 0);
    assert(printf("Child PID: %d \nParent PID: %d\n", childPID, parentPID) != 0);

    if(argc <= 1) {
        assert(printf("Only one argument passed, arguments should be in form" 
        " <file name>, <char array[]>") != 0);
        exit(EXIT_FAILURE);
    }
    
    for(int i = 1; i <= countToThis; i ++) {
        assert(printf("Process ID:  %d %d \n", childPID, i) != 0);
    
    }
    assert(printf("Process %d exited with status: %ld \n", childPID, countToThis) != 0);
    return countToThis;   
}