#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // fork(), getpid()
#include <sys/types.h> // pid_t

int main(void) {
    pid_t childPid;

    switch (childPid = fork()) {
        case -1: // fork() failed
            perror("fork");
        exit(EXIT_FAILURE);

        case 0: // Child process
            printf("I'm the child! My PID is %d\n", getpid());
        // Child does something
        exit(EXIT_SUCCESS);

        default: // Parent process
            printf("I'm the parent! My PID is %d, and I created a child with PID %d\n",
                   getpid(), childPid);
        // Parent does something
        exit(EXIT_SUCCESS);
    }
}
