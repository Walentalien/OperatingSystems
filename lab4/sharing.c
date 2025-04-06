#include <sys/stat.h>  // For mkstemp() and lseek()
#include <fcntl.h>     // For open(), fcntl(), O_APPEND flags
#include <sys/wait.h>  // For wait()

int main(int argc, char *argv[]) {
    int fd, flags;
    char template[] = "/tmp/testXXXXXX";  // Template for temporary file

    setbuf(stdout, NULL); // Disable buffering for stdout to make print statements immediately visible

    // Create a unique temporary file with mkstemp() and open it for reading and writing
    fd = mkstemp(template);
    if (fd == -1) {
        errExit("mkstemp");  // If mkstemp fails, terminate with an error message
    }

    // Print the current file offset in the file
    printf("File offset before fork(): %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

    // Fetch and print the current file status flags (e.g., O_APPEND)
    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        errExit("fcntl - F_GETFL");  // If fcntl fails, terminate with an error message
    }
    printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    // Create a child process using fork()
    switch (fork()) {
    case -1:
        errExit("fork");  // If fork fails, terminate with an error message

    case 0: /* Child process: */
        // Change the file offset (move it to byte 1000)
        if (lseek(fd, 1000, SEEK_SET) == -1) {
            errExit("lseek");
        }

        // Fetch current flags and enable O_APPEND flag
        flags = fcntl(fd, F_GETFL);
        if (flags == -1) {
            errExit("fcntl - F_GETFL");
        }
        flags |= O_APPEND;  // Turn on O_APPEND flag
        if (fcntl(fd, F_SETFL, flags) == -1) {
            errExit("fcntl - F_SETFL");
        }

        // Exit the child process
        _exit(EXIT_SUCCESS);

    default: /* Parent process: */
        // Wait for the child process to finish
        if (wait(NULL) == -1) {
            errExit("wait");  // If wait fails, terminate with an error message
        }

        // Print that the child has exited
        printf("Child has exited\n");

        // Print the current file offset in the parent process
        printf("File offset in parent: %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

        // Fetch and print the current file status flags in the parent
        flags = fcntl(fd, F_GETFL);
        if (flags == -1) {
            errExit("fcntl - F_GETFL");
        }
        printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");

        // Exit the parent process
        exit(EXIT_SUCCESS);
    }
}
