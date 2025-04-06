#include <stdio.h>      // For printf(), perror()
#include <stdlib.h>     // For exit()
#include <string.h>     // For strcmp()
#include <fcntl.h>      // For open() flags
#include <sys/stat.h>   // For file permission constants
#include <unistd.h>     // For read(), write(), close()

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s old-file new-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the input file for reading
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Set flags for output file: create if not exists, write-only, truncate
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // 0666 permissions

    // Open the output file
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1) {
        perror("Error opening output file");
        close(inputFd);
        exit(EXIT_FAILURE);
    }

    // Read from input and write to output
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        if (write(outputFd, buf, numRead) != numRead) {
            perror("Error writing to output file");
            close(inputFd);
            close(outputFd);
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        perror("Error reading input file");
        close(inputFd);
        close(outputFd);
        exit(EXIT_FAILURE);
    }

    if (close(inputFd) == -1) {
        perror("Error closing input file");
        exit(EXIT_FAILURE);
    }

    if (close(outputFd) == -1) {
        perror("Error closing output file");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
