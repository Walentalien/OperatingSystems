// client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define ID_SIZE 32

int sock;
char client_id[ID_SIZE];

void handle_sigint(int sig) {
    send(sock, "STOP", 4, 0);
    close(sock);
    printf("\nDisconnected\n");
    exit(0);
}

void *receive_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        printf("%s\n", buffer);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <id> <server_ip> <server_port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    strncpy(client_id, argv[1], ID_SIZE - 1);
    client_id[ID_SIZE - 1] = '\0';

    char *server_ip = argv[2];
    int port = atoi(argv[3]);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = { .sin_family = AF_INET, .sin_port = htons(port) };
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    signal(SIGINT, handle_sigint);

    send(sock, client_id, strlen(client_id), 0);

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        send(sock, buffer, strlen(buffer), 0);
    }

    return 0;
}
