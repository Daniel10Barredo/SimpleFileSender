/*
 Simple file receiver for OSCP (server)

 Complation:
    gcc -static receiver.c -o receiver64
    gcc -static -m32 receiver.c -o receiver32
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002
#define BUFFER_SIZE 1024

void saveFile(int clientSocket, const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "\nUse: %s <file>\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    printf("");

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    //Socket creation
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror(" [!] Error on socket creation!");
        exit(EXIT_FAILURE);
    }

    // Server configuration
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror(" [!] Error on socket creation!");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    //Start listen
    if (listen(serverSocket, 5) == -1) {
        perror(" [!] Error on listen connections!");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf(" [>] Listen on port %d\n", PORT);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
    if (clientSocket == -1) {
        perror(" [!] Error on accept connection!");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf(" [>] Connection stablished: %s\n", inet_ntoa(clientAddr.sin_addr));
    saveFile(clientSocket, argv[1]);

    // Cerrar conexiones
    close(clientSocket);
    close(serverSocket);

    return 0;
}

void saveFile(int clientSocket, const char *filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror(" [!] Error on file open!");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytesRead, file);
    }
    fclose(file);

    printf(" [>] File save in '%s'\n", filename);
}
