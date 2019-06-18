#include <unistd.h>

// Child sends signals to parent --> parent will contain the signla 
// handlers (all of them) and have them ALL SET UP BEFORE the fork() 
// and execl() to child.) send to:

int main() {
    int returnValue;

    return returnValue;
}
