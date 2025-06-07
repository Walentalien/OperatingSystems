// server 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> // for optinal stuff
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define ID_SIZE 32

typedef struct {
    int socket;
    char id[ID_SIZE];
    int active;
} client_t;

client_t clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
// Global server socket (for cleanup function)
int server_sock = -1;  

void send_to_all(char *message, int from_sock);
void send_to_one(char *id, char *message, int from_sock);
void send_list(int to_sock);
void remove_client(int sock);
void *handle_client(void *arg);
void *alive_pinger(void *arg);

void cleanup_and_exit(int signum) {
    printf("\nShutting down server...\n");
    
    // Close all client connections
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            printf("Closing connection to client: %s\n", clients[i].id);
            // Send shutdown message to client
            char shutdown_msg[] = "Server is shutting down. Goodbye!\n";
            send(clients[i].socket, shutdown_msg, strlen(shutdown_msg), 0);
            close(clients[i].socket);
            clients[i].active = 0;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    // Close server socket
    if (server_sock != -1) {
        close(server_sock);
    }
    
    exit(signum);
}

void timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Set up signal handler
    signal(SIGINT, cleanup_and_exit);
    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int port = atoi(argv[1]);
    struct sockaddr_in server_addr = { .sin_family = AF_INET, .sin_port = htons(port), .sin_addr.s_addr = INADDR_ANY };
    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    // Get and print socket information
    struct sockaddr_in actual_addr;
    socklen_t len = sizeof(actual_addr);
    if (getsockname(server_sock, (struct sockaddr *)&actual_addr, &len) == 0) {
        printf("Server bound to address: %s:%d\n", 
               inet_ntoa(actual_addr.sin_addr), 
               ntohs(actual_addr.sin_port));
    }
    
    listen(server_sock, MAX_CLIENTS);

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, alive_pinger, NULL);

    printf("Server listening on port %d...\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);

        // Get and print client's address information
        struct sockaddr_in peer_addr;
        socklen_t peer_len = sizeof(peer_addr);
        if (getpeername(client_sock, (struct sockaddr *)&peer_addr, &peer_len) == 0) {
            printf("New client connected from: %s:%d\n", 
                   inet_ntoa(peer_addr.sin_addr), 
                   ntohs(peer_addr.sin_port));
        }

        pthread_mutex_lock(&clients_mutex);
        int i;
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (!clients[i].active) {
                clients[i].socket = client_sock;
                clients[i].active = 1;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        if (i < MAX_CLIENTS) {
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, &clients[i]);
        } else {
            char *msg = "Server full\n";
            send(client_sock, msg, strlen(msg), 0);
            close(client_sock);
        }
    }
    return 0;
}

void *handle_client(void *arg) {
    client_t *cli = (client_t *)arg;
    char buffer[BUFFER_SIZE];
    recv(cli->socket, cli->id, ID_SIZE, 0); // receive client ID

    printf("Client connected: %s\n", cli->id);

    while (1) {
        int bytes = recv(cli->socket, buffer, BUFFER_SIZE, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';

        if (strncmp(buffer, "LIST", 4) == 0) {
            send_list(cli->socket);
        } else if (strncmp(buffer, "2ALL ", 5) == 0) {
            char msg[BUFFER_SIZE];
            char timebuf[64];
            timestamp(timebuf, sizeof(timebuf));
            snprintf(msg, sizeof(msg), "[%s] %s: %s", timebuf, cli->id, buffer + 5);
            send_to_all(msg, cli->socket);
        } else if (strncmp(buffer, "2ONE ", 5) == 0) {
            char target[ID_SIZE];
            char msg[BUFFER_SIZE];
            sscanf(buffer + 5, "%s %[^\n]", target, msg);
            char fullmsg[BUFFER_SIZE];
            char timebuf[64];
            timestamp(timebuf, sizeof(timebuf));
            snprintf(fullmsg, sizeof(fullmsg), "[%s] %s to %s: %s", timebuf, cli->id, target, msg);
            send_to_one(target, fullmsg, cli->socket);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            printf("Client %s requested shutdown\n", cli->id);
            break;
        }
    }

    printf("Client disconnected: %s\n", cli->id);
    remove_client(cli->socket);
    close(cli->socket);
    return NULL;
}

void send_list(int to_sock) {
    char list[BUFFER_SIZE] = "Active clients:\n";
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i)
        if (clients[i].active)
            strcat(list, clients[i].id), strcat(list, "\n");
    pthread_mutex_unlock(&clients_mutex);
    send(to_sock, list, strlen(list), 0);
}

void send_to_all(char *message, int from_sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i)
        if (clients[i].active && clients[i].socket != from_sock)
            send(clients[i].socket, message, strlen(message), 0);
    pthread_mutex_unlock(&clients_mutex);
}

void send_to_one(char *id, char *message, int from_sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active && strcmp(clients[i].id, id) == 0) {
            send(clients[i].socket, message, strlen(message), 0);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i)
        if (clients[i].socket == sock)
            clients[i].active = 0;
    pthread_mutex_unlock(&clients_mutex);
}

void *alive_pinger(void *arg) {
    while (1) {
        sleep(10);
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i].active) {
                if (send(clients[i].socket, "ALIVE\n", 6, 0) <= 0) {
                    printf("Client %s not responding, removing\n", clients[i].id);
                    clients[i].active = 0;
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
    return NULL;
}
