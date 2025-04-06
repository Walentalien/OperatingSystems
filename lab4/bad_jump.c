#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

void f() {
    if (setjmp(env) == 0) {
        printf("Inside f(): setting jump point\n");
    } else {
        printf("We jumped back into f(), but f() already returned!\n");
    }
}

int main() {
    f();  // Call f(), it will set the jump point and return

    printf("Back in main(), now trying to longjmp() into f()\n");
    longjmp(env, 1); // Attempt to jump back into f()

    // This line will never be reached
    printf("This line will not be printed\n");

    exit(EXIT_SUCCESS);
}
