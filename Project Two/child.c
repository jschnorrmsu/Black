Child sends signals to parent --> parent will contain the signla handlers (all of them) and have them ALL SET UP BEFORE the fork() and execl() to child.)

send to:

void handler(int) {
    .
    .
    .
    .
    sigaction()

    then wait for the child
    waitpid();
    and then when signals arrive, the signal handler begins doing its thing. 
}