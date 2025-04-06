#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t childPid = fork();

    if (childPid == -1) {
        // Error handling for failed fork
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (childPid == 0) {
        // Code executed by child process
        printf("Child process: PID = %d\n", getpid());
        _exit(EXIT_SUCCESS); // Use _exit() in child
    } else {
        // Code executed by parent process
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), childPid);
        exit(EXIT_SUCCESS);
    }
}
