#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

extern char etext, edata, end;
void signal_handler(int signal);
int main(int argc, char *argv[]) {
    pid_t pid;
    pid_t ppid;
    // always succesfully returns PID of caller
    pid = getpid(); // get the current process ID
    ppid = getppid();

    long page_size = sysconf(_SC_PAGESIZE);  // Get page size
    printf("System page size: %ld bytes\n", page_size);

    printf("My Process ID (PID) is: %d\n", pid);
    printf("My Parent Process ID (PPID) is: %d\n", ppid);

    printf("Address of end of text segment (etext): %p\n", (void*)&etext);
    printf("Address of end of initialized data segment (edata): %p\n", (void*)&edata);
    printf("Address of end of uninitialized data segment (end): %p\n", (void*)&end);


    // Set up the signal handler
    signal(SIGSEGV, signal_handler);

    // This will cause a segmentation fault
    //int *ptr = NULL;
    //*ptr = 10;  // Dereferencing NULL pointer

    void *initial_brk,*new_brk;
    intptr_t increment = 1024 * 1024; // 1mb increase
    initial_brk = sbrk(0); // returns current heap end
    printf("Initial break (heap end): %p\n", initial_brk);

    if (new_brk == (void *)-1) {
        perror("sbrk");
        exit(EXIT_FAILURE);
    }
    printf("Heap increased by 1 MB. New break: %p\n", new_brk);

    if (brk(initial_brk) == -1) {
        perror("brk");
        exit(EXIT_FAILURE);
    }
    printf("Heap size reverted back using brk to initial state.\n");

    for (int j=0; j<argc; j++){
      printf("argv[%d]: %s\n", j, argv[j]);
    }

    char **p;
    for (p = argv; *p != NULL; p++)
        puts(*p);


    // TODO: Why reads only first argument???
    FILE *file = fopen("/proc/self/cmdline", "r");
    if (file == NULL) {
        perror("Failed to open /proc/self/cmdline");
        return 1;
    }

    char buffer[1024]; // Buffer to store the command line arguments
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
    if (len == 0) {
        perror("Failed to read from /proc/self/cmdline");
        fclose(file);
        return 1;
    }

    buffer[len] = '\0'; // Null-terminate the string
    printf("Command-line arguments: %s\n", buffer);

    fclose(file);


    // accessing environment variables from program
    extern char **environ;
    char **ep;
    for (ep = environ; *ep != NULL; ep++){
      puts(*ep);
      }
    //   print location of shell
    printf("%s\n", getenv("SHELL"));
    exit(EXIT_SUCCESS);

}


void signal_handler(int signal) {
    if (signal == SIGSEGV) {
        printf("Caught segmentation fault (SIGSEGV)\n");
        exit(1);  // Exit gracefully
    }
}