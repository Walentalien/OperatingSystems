#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        // Handle fork error
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        printf("Child process: exiting normally\n");
        exit(42);  // Exit with status 42
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);  // Wait for child process to terminate

        // Check if the child terminated normally
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);  // Get exit status of the child
            printf("Child exited with status: %d\n", exit_status);
        } else {
            printf("Child process did not exit normally\n");
        }
    }

    return 0;
}
