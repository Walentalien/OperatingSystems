#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For fork()
#include <sys/wait.h>  // For wait()
#include <string.h>  // For strcmp()

// Function to handle errors and print messages
void errExit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Function to handle invalid argument input for num-children
void usageErr(const char *format, const char *progName) {
    fprintf(stderr, format, progName);
    exit(EXIT_FAILURE);
}

// A simple helper to parse an integer argument
int getInt(const char *str, int flags, const char *paramName) {
    int num = atoi(str);
    if (num <= 0) {
        fprintf(stderr, "Error: %s must be a positive integer\n", paramName);
        exit(EXIT_FAILURE);
    }
    return num;
}

int main(int argc, char *argv[]) {
    int numChildren, j;
    pid_t childPid;

    // Handle case when user provides the "--help" argument
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [num-children]\n", argv[0]);

    // If the number of children is provided as an argument, use it. Otherwise, default to 1.
    numChildren = (argc > 1) ? getInt(argv[1], 0, "num-children") : 1;

    setbuf(stdout, NULL);  // Disable buffering of stdout, ensuring immediate output

    // Loop to create multiple child processes based on the numChildren argument
    for (j = 0; j < numChildren; j++) {
        switch (childPid = fork()) {
            case -1:  // If fork() fails
                errExit("fork");
            case 0:   // Child process
                printf("%d child\n", j);  // Print the child message
            _exit(EXIT_SUCCESS);      // Exit child process successfully
            default:  // Parent process
                printf("%d parent\n", j);  // Print the parent message
            wait(NULL);  // Parent waits for the child to finish
            break;       // Parent continues to the next iteration
        }
    }
    exit(EXIT_SUCCESS);  // Parent exits after processing all child processes
}
