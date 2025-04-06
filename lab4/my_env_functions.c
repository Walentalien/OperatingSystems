#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

/* Simple implementation of setenv() */
int my_setenv(const char *name, const char *value, int overwrite) {
    if (name == NULL || value == NULL) {
        return -1; // Error: invalid input
    }

    char *env_str;
    if (!overwrite && getenv(name) != NULL) {
        return 0; // Do not overwrite existing
    }

    size_t len = strlen(name) + strlen(value) + 2; // '=' + '\0'
    env_str = malloc(len);
    if (env_str == NULL) {
        return -1; // malloc failed
    }

    snprintf(env_str, len, "%s=%s", name, value);
    int res = putenv(env_str); // putenv uses the passed string directly

    // Note: we cannot free env_str because putenv keeps the pointer
    return res;
}

/* Simple implementation of unsetenv() */
int my_unsetenv(const char *name) {
    if (name == NULL || strchr(name, '=') != NULL) {
        return -1; // invalid name
    }

    size_t name_len = strlen(name);
    char **ep = environ;
    while (*ep != NULL) {
        if (strncmp(*ep, name, name_len) == 0 && (*ep)[name_len] == '=') {
            /* Shift the remaining environment entries down */
            char **shift = ep;
            do {
                shift[0] = shift[1];
                shift++;
            } while (*shift != NULL);
            /* Do not increment ep, because new ep[0] needs to be checked */
        } else {
            ep++;
        }
    }
    return 0;
}

/* Test program */
int main(int argc, char *argv[]) {
    printf("Original PATH: %s\n", getenv("PATH"));

    printf("\nSetting TESTVAR to 'hello'...\n");
    my_setenv("TESTVAR", "hello", 1);
    printf("TESTVAR: %s\n", getenv("TESTVAR"));

    printf("\nUnsetting TESTVAR...\n");
    my_unsetenv("TESTVAR");
    printf("TESTVAR after unset: %s\n", getenv("TESTVAR"));

    return 0;
}
