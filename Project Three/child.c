//#define _POSIX_C_SOURCE 200809l
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {

    pid_t childPID = getpid();
    while(1) {
        printf("Awake in %d\n", childPID);
        sleep(1);
    }

    return(0);
}