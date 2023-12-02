/*
 Simple file sender to OSCP (client for linux)

 Complation:
    gcc -static sender_lin.c -o sender64
    gcc -static -m32 sender_lin.c -o sender32
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002
#define BUFFER_SIZE 1024

void sendFile(const char *serverIp, const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "\n  Use: %s <HostIP> <File>\n\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *serverIp = argv[1];
    const char *filename = argv[2];

    sendFile(serverIp, filename);

    return 0;
}

void sendFile(const char *serverIp, const char *filename) {
    int clientSocket;
    struct sockaddr_in serverAddr;
    FILE *file;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    printf("\n");

    //Socket creation
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror(" [!] Error on socket creation.");
        exit(EXIT_FAILURE);
    }

    //Server configurations
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);
    serverAddr.sin_port = htons(PORT);

    //Server connection
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror(" [!] Error on server connection.");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    //Open file
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror(" [!] Error on open file.");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    //Send binary to socket
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(clientSocket, buffer, bytesRead, 0) == -1) {
            perror(" [!] Error on send data.");
            fclose(file);
            close(clientSocket);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);

    printf(" [>] File sended\n");

    // Cerrar conexión
    close(clientSocket);
}
