#define _POSIX_C_SOURCE 200809l
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <syscall.h>

void handler();

int main() {

    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;

}

void handler(int signum) {

}