#include <stdio.h>
#include <unistd.h>  // for execl()
#include <stdlib.h>

int main() {
    printf("Before execl\n");

    // Replacing current process with /bin/ls
    int ret = execl("/bin/ls", "ls", "-l", "/home", (int *)NULL);

    // If execl fails, it returns -1
    if (ret == -1) {
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    // This will never be printed if execl is successful
    printf("After execl\n");

    return 0;
}
