#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    // Define the available options
    // 'a' and 'b' are flags, 'c' expects an argument
    while ((opt = getopt(argc, argv, "abc:")) != -1) {
        switch (opt) {
            case 'a':
                printf("Option a selected\n");
            break;
            case 'b':
                printf("Option b selected\n");
            break;
            case 'c':
                printf("Option c selected with value: %s\n", optarg);  // optarg holds the argument for '-c'
            break;
            case '?': // Invalid option
                printf("Usage: %s [-a] [-b] [-c value]\n", argv[0]);
            exit(1);
            default:
                break;
        }
    }

    // Print any remaining arguments after options are processed
    if (optind < argc) {
        printf("Non-option arguments: ");
        for (int i = optind; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }

    return 0;
}
