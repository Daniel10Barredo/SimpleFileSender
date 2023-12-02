/*
 Simple file sender to OSCP (client for windows)

 Complation:
    x86_64-w64-mingw32-gcc -static sender_win.c -o sender64.exe -lws2_32
    i686-w64-mingw32-gcc sender_win.c -o sender32.exe -lws2_32
*/

#include <stdio.h>
#include <winsock2.h>

#define PORT 9002
#define BUFFER_SIZE 1024

void sendFile(const char *serverIp, const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "\n Use: %s <HostIP> <File>\n\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *serverIp = argv[1];
    const char *filename = argv[2];

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, " [!] Error on wonsock.\n");
        return EXIT_FAILURE;
    }

    sendFile(serverIp, filename);

    WSACleanup();

    return 0;
}


void sendFile(const char *serverIp, const char *filename) {
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    FILE *file;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    printf("\n");

    //Socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        fprintf(stderr, " [!] Error on socket creation: %ld\n", WSAGetLastError());
        return;
    }

    //Server configurations
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);
    serverAddr.sin_port = htons(PORT);

    //Server connection
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        fprintf(stderr, " [!] Error on server connect: %ld\n", WSAGetLastError());
        closesocket(clientSocket);
        return;
    }

    //Open file to send
    fopen_s(&file, filename, "rb");
    if (file == NULL) {
        fprintf(stderr, " [!] Error on open file\n");
        closesocket(clientSocket);
        return;
    }

    //Send file to server
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(clientSocket, buffer, bytesRead, 0) == SOCKET_ERROR) {
            fprintf(stderr, " [!] Error on send data to server: %ld\n", WSAGetLastError());
            fclose(file);
            closesocket(clientSocket);
            return;
        }
    }
    fclose(file);

    printf(" [>] File sended\n");

    //Close connection
    closesocket(clientSocket);
}
