// minimal_udp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <client_id> <server_ip> <port>\n", argv[0]);
        return 1;
    }

    char *client_id = argv[1];
    char *server_ip = argv[2];
    int port = atoi(argv[3]);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    socklen_t addr_len = sizeof(server_addr);

    // Register with server
    char hello[BUFFER_SIZE];
    snprintf(hello, sizeof(hello), "HELLO %s", client_id);
    sendto(sock, hello, strlen(hello), 0, (struct sockaddr*)&server_addr, addr_len);

    char buffer[BUFFER_SIZE];
    ssize_t len = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
    buffer[len] = '\0';
    printf("Server: %s", buffer);

    // Allow sending more commands
    while (1) {
        printf("> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        buffer[strcspn(buffer, "\n")] = '\0';

        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len);

        len = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
        if (len > 0) {
            buffer[len] = '\0';
            printf("Server: %s", buffer);
        }
    }

    close(sock);
    return 0;
}
