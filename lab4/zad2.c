#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int global = 0;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int local = 0;
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {

        printf("child process\n");



        global++;
        local++;

        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n", local, global);

        // doesn't return if success
        int ret = execl("/bin/ls", "ls", argv[1], (char *)NULL);

        if (ret == -1) {
            perror("execl failed");
            exit(EXIT_FAILURE);
        }


    } else {
        printf("parent process\n");


        printf("parent pid = %d, child pid = %d\n", getpid(), pid);

        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Child exit code: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process did not exit normally\n");
        }


        printf("Parent's local = %d, parent's global = %d\n", local, global);
    }

    return EXIT_SUCCESS;
}
