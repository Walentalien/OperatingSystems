// i enabled core dump with ulimit -c unlimited

#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("This program will generate a core dump.\n");

    // Deliberate segmentation fault (NULL pointer dereference)
    int *ptr = NULL;
    *ptr = 42;  // This will cause a segmentation fault

    return 0;
}
