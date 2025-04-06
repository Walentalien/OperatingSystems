#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For vfork(), sleep(), write()
#include <sys/types.h>  // For pid_t

void errExit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int istack = 222;  // Local variable in main function

    switch (vfork()) {  // Create a child process using vfork()
        case -1:
            errExit("vfork");  // If vfork() fails, exit with an error

        case 0:  // This code executes in the child process
            sleep(3);  // Simulate some delay, making the parent wait
        write(STDOUT_FILENO, "Child executing\n", 16);  // Print message
        istack *= 3;  // Modify the istack variable in the child (affects the parent)
        _exit(EXIT_SUCCESS);  // Exit child process without returning to parent

        default:  // This code executes in the parent process
            write(STDOUT_FILENO, "Parent executing\n", 17);  // Print message
        printf("istack=%d\n", istack);  // Print the value of istack (it may be modified by the child)
        exit(EXIT_SUCCESS);  // Exit parent process
    }
}
