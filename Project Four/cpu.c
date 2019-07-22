#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <time.h>
#include "systemcall.h"
#include "eye2eh.c"

/* With Help From John Shapiro.
This program does the followin.
1) Creates handlers for two signals.
2) Create an idle process which will be executed when there is nothing
   else to do.
3) Create a send_signals process that sends a SIGALRM every so often.

The output looks approximately like:

[...]
Sending signal:  14 to process: 35050
Stopping: 35052
---- entering scheduler
Continuing idle: 35052
---- leaving scheduler
Sending signal:  14 to process: 35050
at the end of send_signals
Stopping: 35052
---- entering scheduler
Continuing idle: 35052
---- leaving scheduler
---- entering process_done
Timer died, cleaning up and killing everything
Terminated: 15

---------------------------------------------------------------------------
Add the following functionality.
1) Change the NUM_SECONDS to 20.

2) Take any number of arguments for executables and place each on the
   processes list with a state of NEW. The executables will not require
   arguments themselves.

3) When a SIGALRM arrives, scheduler() will be called; it currently simply
   restarts the idle process. Instead, do the following.
   a) Update the PCB for the process that was interrupted including the
      number of context switches and interrupts it had and changing its
      state from RUNNING to READY.
   b) If there are any NEW processes on processes list, change its state to
      RUNNING, and fork() and execl() it.
   c) If there are no NEW processes, round robin the processes in the
      processes queue that are READY. If no process is READY in the
      list, execute the idle process.

4) When a SIGCHLD arrives notifying that a child has exited, process_done() is
   called.
   a) Add the printing of the information in the PCB including the number
      of times it was interrupted, the number of times it was context
      switched (this may be fewer than the interrupts if a process
      becomes the only non-idle process in the ready queue), and the total
      system time the process took.
   b) Change the state to TERMINATED.
   c) Restart the idle process to use the rest of the time slice.
*/

#define NUM_SECONDS 20
#define ever ;;

enum STATE { NEW, RUNNING, WAITING, READY, TERMINATED, EMPTY };

struct PCB {

    enum STATE state;
    const char *name;   // name of the executable
    int pid;            // process id from fork();
    int ppid;           // parent process id
    int interrupts;     // number of times interrupted
    int switches;       // may be < interrupts
    int started;        // the time this process started
};

typedef enum { false, true } bool;

#define PROCESSTABLESIZE 10
struct PCB processes[PROCESSTABLESIZE];

struct PCB idle;
struct PCB *running;

int sys_time;
int timer;
struct sigaction alarm_handler;
struct sigaction child_handler;
int status;
int process;
int counter;
int process_number;
int found_new;

void bad(int signum) {
    WRITESTRING("bad signal: ");
    WRITEINT(signum, 4);
    WRITESTRING("\n");
}

// cdecl> declare ISV as array 32 of pointer to function(int) returning void
void(*ISV[32])(int) = {
/*       00   01   02   03   04   05   06   07   08   09 */
/*  0 */ bad, bad, bad, bad, bad, bad, bad, bad, bad, bad,
/* 10 */ bad, bad, bad, bad, bad, bad, bad, bad, bad, bad,
/* 20 */ bad, bad, bad, bad, bad, bad, bad, bad, bad, bad,
/* 30 */ bad, bad
};

void ISR (int signum) {
    if (signum != SIGCHLD) {
        kill (running->pid, SIGSTOP);
        WRITESTRING("Stopping: ");
        WRITEINT(running->pid, 6);
        WRITESTRING("\n");
    }

    ISV[signum](signum);
}

void send_signals(int signal, int pid, int interval, int number) {

    for(int i = 1; i <= number; i++) {
        assert(sleep(interval) == 0);
        WRITESTRING("Sending signal: ");
        WRITEINT(signal, 4);
        WRITESTRING(" to process: ");
        WRITEINT(pid, 6);
        WRITESTRING("\n");
        systemcall(kill(pid, signal));
    }

    WRITESTRING("Done sending signals\n");
}

void create_handler(int signum, struct sigaction action, void(*handler)(int)) {

    action.sa_handler = handler;

    if (signum == SIGCHLD) {
        action.sa_flags = SA_NOCLDSTOP | SA_RESTART;
    } 

    else {
        action.sa_flags =  SA_RESTART;
    }

    systemcall(sigemptyset(&action.sa_mask));
    systemcall(sigaction(signum, &action, NULL));
}

void scheduler (int signum) {

    WRITESTRING("---- entering scheduler\n");
    assert(signum == SIGALRM);

    if(running->state == TERMINATED) {
        WRITESTRING ("Continuing idle: ");
        WRITEINT (idle.pid, 6);
        WRITESTRING ("\n");
        running = &idle;
        idle.state = RUNNING;
        idle.started = time(NULL);
        systemcall (kill (idle.pid, SIGCONT));
        running->pid = idle.ppid;
        running->name = idle.name;
    }

    running->state = READY;
    running->interrupts ++;
    process_number = 0;
    found_new = 0;

    while( found_new == 0 && process_number < counter) {
        if(processes[process_number].state == NEW) {
            found_new = 1;
        }

        else {
            process_number ++;
        }
    }

    if(found_new) {
        running = &processes[process_number];
        running->state = RUNNING;
        assert(processes[process_number].started = time(NULL));
        running->interrupts = 0;
        running->switches = 0;
        assert((running->pid = fork()) != -1);
        
        if(running->pid <0) {
            perror("fork()");
            exit(-1);
        }

        else if(running->pid ==0) {
            int exit_status = execl(running->name, running->name, NULL);
            perror("execl() failed \n");
            exit(exit_status);
        }

        if(running->pid != processes[process_number].pid) {
            processes[process_number].switches ++;
        }
    }

    else {
        if(process == counter) {
            process = 0;
        }

        if(processes[process].state == READY) {
            if(running->pid != processes[process].pid) {
                running->switches++;
            }

            running = &processes[process];
            systemcall(kill(running->pid, SIGCONT));
            process++;
        }
    }

    sleep(1);
    WRITESTRING("---- leaving scheduler\n");
}

void process_done (int signum) {
    WRITESTRING("---- entering process_done\n");
    assert (signum == SIGCHLD);

    if(running->pid != idle.pid) {
        running->state = TERMINATED;
        WRITESTRING("\nProcess: ");
        WRITEINT(running->pid, 7);
        WRITESTRING("\nInterrupts: ");
        WRITEINT(running->interrupts, 7);
        WRITESTRING("\nTime: ");
        WRITEINT(time(NULL) - running->started, 10);
    }

    else {
        WRITESTRING ("Timer died, cleaning up and killing everything\n");
    }

    WRITESTRING("---- leaving proces_done\n");
    systemcall(kill(idle.pid, SIGCONT));
}

void boot()
{
    sys_time = 0;

    ISV[SIGALRM] = scheduler;
    ISV[SIGCHLD] = process_done;
    create_handler(SIGALRM, alarm_handler, ISR);
    create_handler(SIGCHLD, child_handler, ISR);

    assert((timer = fork()) != -1);
    if (timer == 0) {
        send_signals(SIGALRM, getppid(), 1, NUM_SECONDS);
        exit(0);
    }
}

void create_idle() {
    idle.state = READY;
    idle.name = "IDLE";
    idle.ppid = getpid();
    idle.interrupts = 0;
    idle.switches = 0;
    idle.started = sys_time;

    assert((idle.pid = fork()) != -1);
    if (idle.pid == 0) {
        systemcall(pause());
    }
}

int main(int argc, char **argv) {


    boot();
    create_idle();
    running = &idle;


    for(int i = 0; i < argc; i++) {
        processes[i].name = argv[i+1];
        processes[i].state = NEW;
        WRITESTRING("Process ");
        WRITESTRING(processes[i-1].name);
        WRITESTRING("\n");
    }

    for(ever) {
        pause();
    }
}