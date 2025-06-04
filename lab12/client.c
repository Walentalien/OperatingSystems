// client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFFER_SIZE 1024
int sock;
struct sockaddr_in server_addr;
socklen_t addr_len;

void handle_sigint(int sig) {
    sendto(sock, "STOP", 4, 0, (struct sockaddr *)&server_addr, addr_len);
    printf("\nDisconnected.\n");
    close(sock);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <client_id> <server_ip> <port>\n", argv[0]);
        return 1;
    }

    const char *client_id = argv[1];
    const char *server_ip = argv[2];
    int port = atoi(argv[3]);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    addr_len = sizeof(server_addr);

    signal(SIGINT, handle_sigint);

    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "HELLO %s", client_id);
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len);

    recvfrom(sock, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
    printf("%s", buffer);

    while (1) {
        printf("> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        buffer[strcspn(buffer, "\n")] = '\0';

        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len);

        ssize_t len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
        if (len > 0) {
            buffer[len] = '\0';
            printf("Server: %s", buffer);
        }
    }

    return 0;
}


// > LIST
// > 2ALL Hello everyone!
// > 2ONE Bob Private message
// > STOP