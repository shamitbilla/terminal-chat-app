#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 3001
#define BACKLOG 5

int createSocket()
{
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0)
    {
        printf("Error while creating a socket \n");
        return -1;
    }
    printf("Socket created successfully \n");
    return socketFd;
}

struct sockaddr_in createIPv4Addr(char *ip, int port)
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    return serverAddr;
}

// Struct to hold client information
typedef struct
{
    int socketFd;
    struct sockaddr_in address;
} ClientInfo;

// Global array to hold information of connected clients
ClientInfo clients[BACKLOG];
int numClients = 0;

// Function to broadcast message to all clients except the sender
void broadcastMessage(char *message, int senderSocketFd)
{
    for (int i = 0; i < numClients; i++)
    {
        if (clients[i].socketFd != senderSocketFd)
        {
            send(clients[i].socketFd, message, strlen(message), 0);
        }
    }
    memset(message,'\0',sizeof(message));
}

// Function to handle communication with each client
void *handleClient(void *clientSocket)
{
    int clientSocketFd = *((int *)clientSocket);
    char client_message[2000];
    char server_message[2000];

    memset(server_message, '\0', sizeof(server_message));
    strcpy(server_message, "Successfully connected to the server!");
    send(clientSocketFd, server_message, strlen(server_message), 0);
    memset(server_message, '\0', sizeof(server_message));

    while (1)
    {
        memset(client_message, '\0', sizeof(client_message)); // Clear client_message buffer

        // Receive message from client
        int bytes_received = recv(clientSocketFd, client_message, sizeof(client_message), 0);
        if (bytes_received <= 0)
        {
            printf("Client disconnected\n");
            // Remove client from the list
            for (int i = 0; i < numClients; i++)
            {
                if (clients[i].socketFd == clientSocketFd)
                {
                    for (int j = i; j < numClients - 1; j++)
                    {
                        clients[j] = clients[j + 1];
                    }
                    numClients--;
                    break;
                }
            }
            close(clientSocketFd);
            return NULL;
        }

        printf("Msg from Client: %s\n", client_message);

        // Broadcast the message to all clients
        broadcastMessage(client_message, clientSocketFd);
    }

    close(clientSocketFd);
    return NULL;
}

int main()
{
    char server_message[2000];

    memset(server_message, '\0', sizeof(server_message));

    int socketFd = createSocket();
    struct sockaddr_in clientAddr;

    struct sockaddr_in serverAddr = createIPv4Addr("127.0.0.1", PORT);

    if (bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Error while binding to port \n");
        return -1;
    }

    printf("Socket bound successfully \n");

    if (listen(socketFd, BACKLOG) < 0)
    {
        printf("Error while listening \n");
        return -1;
    }

    printf("\n Listening for incoming connections at port %d\n", PORT);

    int clientAddrSize = sizeof(clientAddr);

    // Accept incoming connections in a loop
    while (1)
    {
        int clientSocketFd = accept(socketFd, (struct sockaddr *)&clientAddr, &clientAddrSize);

        if (clientSocketFd < 0)
        {
            printf("Cannot accept the incoming request \n");
            continue;
        }

        // Add client to the list
        clients[numClients].socketFd = clientSocketFd;
        clients[numClients].address = clientAddr;
        numClients++;

        printf("Client connected at IP : %s and port %i\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // Create a new thread to handle the client
        pthread_t tid;
        if (pthread_create(&tid, NULL, handleClient, &clientSocketFd) != 0)
        {
            printf("Failed to create thread for client\n");
            close(clientSocketFd);
        }
        else
        {
            pthread_detach(tid);
        }
    }

    close(socketFd);

    return 0;
}
