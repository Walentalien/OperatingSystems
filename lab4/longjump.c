#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

static void f2(void) {
    longjmp(env, 2);
}

static void f1(int argc) {
    if (argc == 1)
        longjmp(env, 1);
    f2();
}

int main(int argc, char *argv[]) {
    switch (setjmp(env)) {
        case 0:
            /* This is the point where setjmp() is first called */
                printf("Calling f1() after initial setjmp()\n");
        f1(argc); /* f1() will either call longjmp(env, 1) or f2() */
        break;
        case 1:
            printf("We jumped back from f1()\n");
        break;
        case 2:
            printf("We jumped back from f2()\n");
        break;
    }

    exit(EXIT_SUCCESS);
}
