#include <stdio.h>
#include <time.h>

void timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

int main(void) {
    char current[20];
    timestamp(current, sizeof(current));
    printf("Current time: %s\n", current);
    return 0;
}