#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_children>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numChildren = atoi(argv[1]);
    pid_t childPid;

    for (int i = 0; i < numChildren; i++) {
        childPid = fork();

        if (childPid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (childPid == 0) {
            printf("Parent PID: %d, Child PID: %d\n", getppid(), getpid());
            exit(EXIT_SUCCESS);
        }
    }


    for (int i = 0; i < numChildren+10; i++) {
        wait(0);
    }

    printf("Number of processes spawned: %d\n", numChildren);
    exit(EXIT_SUCCESS);
}
