#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT "8080"
#define BUFFER_SIZE 512

int main() {
    WSADATA wsaData;
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    struct addrinfo hints, *result = NULL;
    char recvbuf[BUFFER_SIZE];
    int recvbuflen = BUFFER_SIZE;
    int run = 1; // Tracks the message turn

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    if (getaddrinfo(NULL, PORT, &hints, &result) != 0) {
        printf("getaddrinfo failed\n");
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        printf("socket failed\n");
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        printf("bind failed\n");
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen failed\n");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("Server is running and listening on port %s...\n", PORT);
    printf("Launch your client program now!\n\n");

    // Accept a client socket
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed\n");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server listen socket
    closesocket(listenSocket);

    // Communication loop
    while (1) {
        // Clear buffer and wait for client message
        ZeroMemory(recvbuf, BUFFER_SIZE);
        int iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        
        if (iResult > 0) {
            printf("[Run %d] Client says: %s\n", run, recvbuf);

            char sendbuf[BUFFER_SIZE] = "";

            // --- TASK TRIGGER LOGIC ---
            
            if (run == 5) {
                // Client expects an ID lookup trigger. We just send something back.
                strcpy(sendbuf, "Triggering ID function task!");
            } 
            else if (run == 8) {
                // Client decodes 131 bytes using ^ 55.
                // Let's hide a message inside 131 characters!
                char *secretMessage = "XOR puzzle solved! You successfully decoded this hidden transmission using bitwise operations.";
                
                // Fill buffer with spaces first, then copy secret message
                memset(sendbuf, ' ', 131); 
                memcpy(sendbuf, secretMessage, strlen(secretMessage));
                
                // XOR obfuscate the entire 131-byte array with 55 so your client decodes it properly
                for (int i = 0; i < 131; i++) {
                    sendbuf[i] = sendbuf[i] ^ 55;
                }
            } 
            else if (run == 13) {
                // Client prints characters ONLY at 1-based prime indices: 2, 3, 5, 7, 11, 13, 17, 19...
                // Index mapping (0-based): i+1 must be prime.
                // Let's construct a string where primes spell out "PRIME" and filler characters are '.'
                // indices:  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
                // primes:     * * * * * * *
                // letters:    P R   I   M        E     !        .
                strcpy(sendbuf, ".PR.I.M....E.!.X."); 
            } 
            else {
                // Standard default message for any other round
                sprintf(sendbuf, "Keep going! This is message turn %d.", run);
            }

            // Send the response back to client
            int bytesToSend = (run == 8) ? 131 : (int)strlen(sendbuf);
            send(clientSocket, sendbuf, bytesToSend, 0);
            
            run++; // Move to next turn
        } 
        else if (iResult == 0) {
            printf("Client closed the connection.\n");
            break;
        } 
        else {
            printf("recv failed\n");
            break;
        }
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
