// server
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

int find_client_by_id(const char *id) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

void broadcast(const char *msg, int sender_idx) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && i != sender_idx) {
            sendto(STDOUT_FILENO, msg, strlen(msg), 0,
                   (struct sockaddr *)&clients[i].addr,
                   sizeof(clients[i].addr));
        }
    }
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

    printf("UDP Chat Server started on port %s\n", argv[1]);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        char buffer[BUFFER_SIZE];

        ssize_t len = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0,
                               (struct sockaddr*)&client_addr, &addr_len);
        if (len <= 0) continue;
        buffer[len] = '\0';

        int idx = find_client_by_addr(&client_addr);

        if (strncmp(buffer, "HELLO ", 6) == 0) {
            char *name = buffer + 6;
            if (idx == -1) {
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
            sendto(sock, "WELCOME\n", 8, 0, (struct sockaddr*)&client_addr, addr_len);
        } else if (idx >= 0) {
            if (strcmp(buffer, "LIST") == 0) {
                char list[BUFFER_SIZE] = "Active clients:\n";
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i].active) {
                        strcat(list, clients[i].id);
                        strcat(list, "\n");
                    }
                }
                sendto(sock, list, strlen(list), 0, (struct sockaddr*)&client_addr, addr_len);
            } else if (strncmp(buffer, "2ALL ", 5) == 0) {
                char msg[BUFFER_SIZE];
                snprintf(msg, sizeof(msg), "%s (broadcast): %s\n", clients[idx].id, buffer + 5);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i].active && i != idx) {
                        sendto(sock, msg, strlen(msg), 0,
                               (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
                    }
                }
            } else if (strncmp(buffer, "2ONE ", 5) == 0) {
                char target_id[ID_SIZE], msg[BUFFER_SIZE];
                sscanf(buffer + 5, "%s %[^\n]", target_id, msg);
                int target = find_client_by_id(target_id);
                if (target >= 0) {
                    char full_msg[BUFFER_SIZE];
                    snprintf(full_msg, sizeof(full_msg), "%s (private): %s\n",
                             clients[idx].id, msg);
                    sendto(sock, full_msg, strlen(full_msg), 0,
                           (struct sockaddr*)&clients[target].addr, sizeof(clients[target].addr));
                } else {
                    char error_msg[] = "Target client not found.\n";
                    sendto(sock, error_msg, strlen(error_msg), 0,
                           (struct sockaddr*)&client_addr, addr_len);
                }
            } else if (strcmp(buffer, "STOP") == 0) {
                printf("Client %s disconnected.\n", clients[idx].id);
                clients[idx].active = 0;
                sendto(sock, "BYE\n", 4, 0,
                       (struct sockaddr*)&client_addr, addr_len);
            } else {
                sendto(sock, "Unknown command\n", 16, 0,
                       (struct sockaddr*)&client_addr, addr_len);
            }
        }
    }

    return 0;
}
