#include <fcntl.h>      // For open(), lseek()
#include <unistd.h>     // For read(), write(), close()
#include <stdlib.h>     // For exit()
#include <stdio.h>      // For perror()
#include <string.h>     // For memset()
#include <sys/stat.h>   // For file permission macros

#ifndef BUF_SIZE
#define BUF_SIZE 4096
#endif

int main(int argc, char *argv[]) {
    int inputFd, outputFd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s source_file dest_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        perror("open source");
        exit(EXIT_FAILURE);
    }

    outputFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (outputFd == -1) {
        perror("open destination");
        exit(EXIT_FAILURE);
    }

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        ssize_t i = 0;

        while (i < numRead) {
            // Find start of non-zero data
            ssize_t start = i;
            while (start < numRead && buf[start] == '\0')
                start++;

            // Seek over zeros (hole)
            if (start > i) {
                if (lseek(outputFd, start - i, SEEK_CUR) == -1) {
                    perror("lseek");
                    exit(EXIT_FAILURE);
                }
            }

            // Find end of non-zero data
            ssize_t end = start;
            while (end < numRead && buf[end] != '\0')
                end++;

            // Write non-zero data
            if (end > start) {
                if (write(outputFd, &buf[start], end - start) != end - start) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }

            i = end;
        }
    }

    if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    if (close(inputFd) == -1) {
        perror("close input");
        exit(EXIT_FAILURE);
    }
    if (close(outputFd) == -1) {
        perror("close output");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
