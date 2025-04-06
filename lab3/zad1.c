#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>     // opendir(), readdir(), closedir()
#include <sys/stat.h>   // stat()
#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()

#define BUF_SIZE 1024

void copy_file(const char *src_path, const char *dest_path) {
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        perror("open src");
        return;
    }

    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("open dest");
        close(src_fd);
        return;
    }

    char buf[BUF_SIZE];
    ssize_t num_read;
    while ((num_read = read(src_fd, buf, BUF_SIZE)) > 0) {
        if (write(dest_fd, buf, num_read) != num_read) {
            perror("write");
            break;
        }
    }

    if (num_read == -1)
        perror("read");

    close(src_fd);
    close(dest_fd);
}

int ends_with_txt(const char *filename) {
    size_t len = strlen(filename);
    return len >= 4 && strcmp(filename + len - 4, ".txt") == 0;
}

int is_hidden(const char *filename) {
  return filename[0] == '.';
}

void reverse_line(char *line) {
    size_t len = strlen(line);
    // Remove newline before reversing if exists
    if (len > 0 && line[len-1] == '\n') {
        len--;
    }
    for (size_t i = 0; i < len/2; i++) {
        char temp = line[i];
        line[i] = line[len-1-i];
        line[len-1-i] = temp;
    }
}

void reverse_lines_in_file(const char *filepath) {
    FILE *fp = fopen(filepath, "r+");
    if (fp == NULL) {
        perror("fopen");
        return;
    }

    char **lines = NULL;
    size_t lines_alloc = 0, lines_used = 0;

    char buf[BUF_SIZE];
    while (fgets(buf, sizeof(buf), fp)) {
        if (lines_used >= lines_alloc) {
            lines_alloc = lines_alloc ? lines_alloc * 2 : 10;
            lines = realloc(lines, lines_alloc * sizeof(char *));
            if (lines == NULL) {
                perror("realloc");
                fclose(fp);
                return;
            }
        }
        lines[lines_used++] = strdup(buf);
    }

    // Go back to start and overwrite
    rewind(fp);

    for (size_t i = 0; i < lines_used; i++) {
        reverse_line(lines[i]);
        fputs(lines[i], fp);
        free(lines[i]);
    }

    // Truncate file if new content is shorter
    ftruncate(fileno(fp), ftell(fp));

    free(lines);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <dir1> <dir2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *src_dir = argv[1];
    const char *dest_dir = argv[2];

    DIR *dirp = opendir(src_dir);
    if (dirp == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG && ends_with_txt(entry->d_name) && !is_hidden(entry->d_name) ) {
            // build full paths
            char src_path[4096];
            char dest_path[4096];

            snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
            snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);

            copy_file(src_path, dest_path);
            printf("Copied: %s -> %s\n", src_path, dest_path);
        }
    }

    closedir(dirp);

    // Now reverse lines in dest_dir
    DIR *destp = opendir(dest_dir);
    if (destp == NULL) {
        perror("opendir dest_dir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(destp)) != NULL) {
        if (entry->d_type == DT_REG && ends_with_txt(entry->d_name)) {
            char dest_path[4096];
            snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);

            reverse_lines_in_file(dest_path);
            printf("Reversed lines in: %s\n", dest_path);
        }
    }

    closedir(destp);

    return 0;
}
