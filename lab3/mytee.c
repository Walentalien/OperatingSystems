#include <stdio.h>      // For perror(), printf()
#include <stdlib.h>     // For exit()
#include <fcntl.h>      // For open() flags
#include <unistd.h>     // For read(), write(), close()
#include <string.h>     // For strcmp()
#include <sys/stat.h>   // For file permission constants
#include <getopt.h>     // For getopt()

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
    int opt;
    int appendMode = 0;
    int outputFd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    // Parse command-line options
    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch (opt) {
            case 'a':
                appendMode = 1;
            break;
            default:
                fprintf(stderr, "Usage: %s [-a] file\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Expected file name after options\n");
        exit(EXIT_FAILURE);
    }

    // Open the file
    int openFlags = O_WRONLY | O_CREAT;
    openFlags |= appendMode ? O_APPEND : O_TRUNC;

    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // rw-r--r--

    outputFd = open(argv[optind], openFlags, filePerms);
    if (outputFd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read from standard input, write to both stdout and file
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, numRead) != numRead) {
            perror("write to stdout");
            exit(EXIT_FAILURE);
        }
        if (write(outputFd, buf, numRead) != numRead) {
            perror("write to file");
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    if (close(outputFd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
