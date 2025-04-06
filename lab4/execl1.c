#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Before execl\n");

    // Execute 'ps aux' to list all processes
    execl("/bin/ps", "ps", NULL);
    printf("After execl"); // this line wont be executed
    // This line will not be executed if execl is successful
    perror("execl failed");
    return 0;
}
