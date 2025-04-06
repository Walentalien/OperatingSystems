#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
// a program to see fvork chlild closes file descriptor, which doesnt affect file descriptor of parent
int main() {
    pid_t childPid;
    int fd;

    // Open a file descriptor for standard input (fd 0)
    fd = dup(0); // duplicate the standard input
    if (fd == -1) {
        perror("dup");
        exit(EXIT_FAILURE);
    }

    // Print the initial status of the file descriptor
    printf("Parent: File descriptor before vfork() is %d\n", fd);

    switch (childPid = vfork()) {
        case -1:
            perror("vfork");
        exit(EXIT_FAILURE);

        case 0: /* Child process */
            printf("Child: Closing file descriptor %d\n", fd);
        close(fd);  // Close the file descriptor in the child process

        // Child process terminates
        _exit(EXIT_SUCCESS);

        default: /* Parent process */
            // Parent waits for the child to finish
                if (wait(NULL) == -1) {
                    perror("wait");
                    exit(EXIT_FAILURE);
                }

        // Check if the parent's file descriptor is still valid
        printf("Parent: File descriptor after child terminates is %d\n", fd);
        // In the parent, file descriptor 0 should still be open
        if (fd == 0) {
            printf("Parent: Standard input (fd 0) is still open.\n");
        } else {
            printf("Parent: File descriptor is not 0 anymore.\n");
        }

        exit(EXIT_SUCCESS);
    }
}
