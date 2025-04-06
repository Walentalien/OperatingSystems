#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define SYNC_SIG SIGUSR1 /* Synchronization signal */

static void handler(int sig) {
    // Signal handler function does nothing, but it's required to be there
    // to handle SIGUSR1.
}

char* currTime(const char* format) {
    static char timeStr[100];  // Static buffer to hold formatted time string
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(timeStr, sizeof(timeStr), format, tm_info);
    return timeStr;
}

void errExit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    pid_t childPid;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    setbuf(stdout, NULL); /* Disable buffering of stdout */

    // Block the synchronization signal (SIGUSR1)
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);
    if (sigpr#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define SYNC_SIG SIGUSR1 /* Synchronization signal */

static void handler(int sig) {
    // Signal handler function does nothing, but it's required to be there
    // to handle SIGUSR1.
}

char* currTime(const char* format) {
    static char timeStr[100];  // Static buffer to hold formatted time string
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(timeStr, sizeof(timeStr), format, tm_info);
    return timeStr;
}

void errExit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    pid_t childPid;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    setbuf(stdout, NULL); /* Disable buffering of stdout */

    // Block the synchronization signal (SIGUSR1)
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);
    if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
        errExit("sigprocmask");

    // Set up the signal handler for SIGUSR1
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    switch (childPid = fork()) {
        case -1:
            errExit("fork");

        case 0: /* Child process */
            // Simulate some work in the child
            printf("[%s %ld] Child started - doing some work\n", currTime("%T"), (long) getpid());
            sleep(2); /* Simulate work by sleeping for 2 seconds */

            // Child signals the parent that it is done
            printf("[%s %ld] Child about to signal parent\n", currTime("%T"), (long) getpid());
            if (kill(getppid(), SYNC_SIG) == -1)
                errExit("kill");

            // The child can continue doing other things...
            _exit(EXIT_SUCCESS);

        default: /* Parent process */
            // Parent does some work, and then waits for the signal from the child
            printf("[%s %ld] Parent about to wait for signal\n", currTime("%T"), (long) getpid());

            // Wait for the child to signal the parent by using sigsuspend
            sigemptyset(&emptyMask);
            if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
                errExit("sigsuspend");

            // The parent received the signal
            printf("[%s %ld] Parent got signal\n", currTime("%T"), (long) getpid());

            // Reset signal mask to its original state after handling the signal
            if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
                errExit("sigprocmask");

            // Parent can now continue with its work...
            exit(EXIT_SUCCESS);
    }
}
ocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
        errExit("sigprocmask");

    // Set up the signal handler for SIGUSR1
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    switch (childPid = fork()) {
        case -1:
            errExit("fork");

        case 0: /* Child process */
            // Simulate some work in the child
            printf("[%s %ld] Child started - doing some work\n", currTime("%T"), (long) getpid());
            sleep(2); /* Simulate work by sleeping for 2 seconds */

            // Child signals the parent that it is done
            printf("[%s %ld] Child about to signal parent\n", currTime("%T"), (long) getpid());
            if (kill(getppid(), SYNC_SIG) == -1)
                errExit("kill");

            // The child can continue doing other things...
            _exit(EXIT_SUCCESS);

        default: /* Parent process */
            // Parent does some work, and then waits for the signal from the child
            printf("[%s %ld] Parent about to wait for signal\n", currTime("%T"), (long) getpid());

            // Wait for the child to signal the parent by using sigsuspend
            sigemptyset(&emptyMask);
            if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
                errExit("sigsuspend");

            // The parent received the signal
            printf("[%s %ld] Parent got signal\n", currTime("%T"), (long) getpid());

            // Reset signal mask to its original state after handling the signal
            if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
                errExit("sigprocmask");

            // Parent can now continue with its work...
            exit(EXIT_SUCCESS);
    }
}
