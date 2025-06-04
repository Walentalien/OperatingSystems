// minimal_udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define ID_SIZE 32

typedef struct {
    char id[ID_SIZE];
    struct sockaddr_in addr;
    int active;
} client_t;

client_t clients[MAX_CLIENTS];

int find_client_by_addr(struct sockaddr_in *addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active &&
            clients[i].addr.sin_addr.s_addr == addr->sin_addr.s_addr &&
            clients[i].addr.sin_port == addr->sin_port) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr.s_addr = INADDR_ANY
    };
    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("UDP Server started on port %s\n", argv[1]);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        char buffer[BUFFER_SIZE];

        ssize_t len = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0,
                               (struct sockaddr*)&client_addr, &addr_len);
        if (len <= 0) continue;
        buffer[len] = '\0';

        printf("Received: %s\n", buffer);

        if (strncmp(buffer, "HELLO ", 6) == 0) {
            // Register client
            char *name = buffer + 6;
            int already = find_client_by_addr(&client_addr);

            if (already == -1) {
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (!clients[i].active) {
                        strncpy(clients[i].id, name, ID_SIZE - 1);
                        clients[i].addr = client_addr;
                        clients[i].active = 1;
                        printf("Registered: %s\n", name);
                        break;
                    }
                }
            }

            sendto(sock, "WELCOME\n", 8, 0,
                   (struct sockaddr*)&client_addr, addr_len);
        } else if (strcmp(buffer, "LIST") == 0) {
            char reply[BUFFER_SIZE] = "Active clients:\n";
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].active) {
                    strcat(reply, clients[i].id);
                    strcat(reply, "\n");
                }
            }
            sendto(sock, reply, strlen(reply), 0,
                   (struct sockaddr*)&client_addr, addr_len);
        } else {
            char *msg = "Unknown command\n";
            sendto(sock, msg, strlen(msg), 0,
                   (struct sockaddr*)&client_addr, addr_len);
        }
    }

    return 0;
}
