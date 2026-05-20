#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 4096
int prime(int lenght) {
    if (lenght <= 1) return 0;
    if (lenght == 2) return 1;
    if (lenght == 3) return 1;
    for (int i = 2; i * i <= lenght; i++) {
        if (lenght % i == 0) return 0;
    }
    return 1;
}

void colour(int colour_number) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour_number);
}

int idfunction(int id) {
    int Result=0;
    char id_str[20];
    sprintf(id_str, "%d", id);
    int first_digit=id_v_str[0]-'0';
    int second_digit=id_v_str[1]-'0';
    int third_digit=id_v_str[2]-'0';
    int fourth_digit=id_v_str[3]-'0';
    int fifth_digit=id_v_str[4]-'0';
    if (fifth_digit==0) {
        fifth_digit=9;
    }
    Result=first_digit+second_digit+third_digit+fourth_digit+fifth_digit;
	Result=Result%fifth_digit;
    return Result;
}


int main() {
	const char* file_name = "file.txt";
    FILE *logFile = fopen(file_name, "a");// Opening file to log in the conversation
    SetConsoleOutputCP(CP_UTF8);
    char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;


    WSADATA wsaData; // WSADATA to work with Winsock

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); //First initialization
    if (iResult != 0) //Error check
    {

        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }


    struct addrinfo *result = NULL, *ptr = NULL; //Structure for working with addresses
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP; //Work with protocols TCP/IP

    // Resolve the server address and port
	const char* ip = "127.0.0.1";
	const char* port = "111"
    iResult = getaddrinfo(ip, port, &hints, &result);
    if (iResult != 0) //Error Check
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    else
        printf("getaddrinfo didn't fail…\n");
    // Creating the Socket

    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned
    ptr = result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("Unable to connect to server! Error: %ld\n", WSAGetLastError());
        closesocket(ConnectSocket);
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    if (ConnectSocket == INVALID_SOCKET) //Error checl
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    else
        printf("Error at socket DIDN'T occur…\n");

    // Connect to server
    int run=0;
    while (run<100) {
        if (run==5) {// Use on id to find a hidden code
			int idnumber=0;//would use my own id number
            printf("The number from id %d", idfunction(idnumber));
        }
        if (run==8) {
            for (int i=0; i<131; i++) {
                printf("%c",recvbuf[i] ^ 55);
            }
        }
        if (run == 13) {// Printing out only prime numbers from the message
            for (int i = 0; i < strlen(recvbuf); i++) {
                if (prime(i+1)) {
                    printf("%c", recvbuf[i]);
                }
            }
            printf("\n");
        }
        farba(1);
        printf("Me : ");

        // Get input from terminal
        if (fgets(sendbuf, DEFAULT_BUFLEN, stdin) == NULL) break;

        // Remove \n
        sendbuf[strcspn(sendbuf, "\r\n")] = 0;

        // Quitting
        if (strcmp(sendbuf, "quit") == 0) {
            printf("Closing connection...\n");
            break;
        }


        if (strlen(sendbuf) == 0) continue;

        // Sending
        farba(1);
        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        fprintf(logFile, "Me: %s\n", sendbuf);
        fflush(logFile);
        if (iResult == SOCKET_ERROR) {
            printf("Send failed: %d\n", WSAGetLastError());
            break;
        }


        memset(recvbuf, 0, DEFAULT_BUFLEN);
        iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN - 1, 0);
        fprintf(logFile, "morpheus: %s\n", recvbuf);
        fflush(logFile);

        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            recvbuf[strcspn(recvbuf, "\r\n")] = 0; // Clean string


            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;


            int padding_size = columns / 2;
            int max_text_width = (columns - padding_size) - 10; // Max chars per line

            char *ptr = recvbuf;
            int first_line = 1;

            farba(2);

            while (*ptr != '\0') {
                while (*ptr == ' ') ptr++;
                if (*ptr == '\0') break;
                printf("%*s", padding_size, "");
                if (first_line) {
                    printf("Morpheus: ");
                    first_line = 0;
                } else {
                    printf("          "); // Align with "Morpheus: " (10 spaces)
                }


                int lenght_of_line = (int)strlen(ptr);
                int print_now = lenght_of_line;

                if (lenght_of_line > max_text_width) {
                    print_now = max_text_width;

                    while (print_now > 0 && ptr[print_now] != ' ' && ptr[print_now] != '\0') {
                        print_now--;
                    }

                    if (print_now == 0) print_now = max_text_width;
                }

                // Print the calculated chunk
                printf("%.*s\n", print_now, ptr);

                // Move the pointer forward
                ptr += print_now;
            }
        } else if (iResult == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            printf("Recv failed: %d\n", WSAGetLastError());
            break;
        }
        run++;
    }
    closesocket(ConnectSocket);
    WSACleanup();
    fclose(logFile);
}