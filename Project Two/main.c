#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>

static void handler() {
    
}

int main() {

    pid_t childPID = fork();
    int exitStatus;

    if( childPID < 0) {
        perror("System call fork() failed\n");
        exit(-1);
    }

    else if( childPID == 0 ) {
        exitStatus = execl("./child", "child", NULL);
        perror("Function execl() failed\n");
        exit(exitStatus);
    }

    else {
        assert(waitpid(childPID, &exitStatus, 0) != -1);
    }

    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = SA_RESTART;
    assert (sigaction (SIGUSR1, &action, NULL) == 0 );
    
    return 0;

} 