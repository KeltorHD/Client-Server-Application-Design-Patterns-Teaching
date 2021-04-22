#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS



/*
* TCP Client
*/
#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>

// TCP-порт клиента

#define DEFAULT_BUFLEN 512
#define SERVER_PORT "20002"

void clean(char* src);

int main()
{

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    const char* sendbuf = "Hello, server!";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", SERVER_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    /*server hello*/
    //iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    //clean(recvbuf);

    //if (iResult > 0)
    //printf("Server welcome: %s\n", recvbuf);

    std::cout << "DONE" << std::endl;

    try
    {
        /*send info*/
        std::string xml{};
        std::ifstream file("authorization.xml");
        std::string tmp;
        while (std::getline(file, tmp))
        {
            xml += tmp;
        }
        std::cout << xml << std::endl;

        /*send info*/
        int32_t var = xml.length();
        var++;
        char buf[4];
        std::memcpy(buf, &var, 4);

        iResult = send(ConnectSocket, buf, 4, 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        printf("Send \"info\", bytes Sent: %ld\n", iResult);

        iResult = send(ConnectSocket, xml.c_str(), xml.length(), 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
        printf("Send \"info\", bytes Sent: %ld\n", iResult);

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        //clean(recvbuf);

        if (iResult > 0)
            printf("Server answer: %s\n", recvbuf);
    }
    catch (int& er)
    {
        if (er == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
}

void clean(char* src)
{
    for (char* pPtr = src; *pPtr != 0; pPtr++)
    {
        if (*pPtr == '\n' || *pPtr == '\r')
        {
            *pPtr = 0;
            break;
        }
    }
}