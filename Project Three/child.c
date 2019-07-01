//#define _POSIX_C_SOURCE 200809l
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <stdio.h>

int main() {

    pid_t childPID = getpid();

    while(1) {
        assert(printf("Awake in %d\n", childPID) != 0);
        assert(sleep(1) >= 0);
    }

    return(0);
}