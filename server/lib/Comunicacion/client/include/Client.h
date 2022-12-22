#ifndef CLIENT_H
#define CLIENT_H

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

class Client
{
    private:
        WSADATA WSAData;
        SOCKET server;
        sockaddr_in clientService;
        hostent *localHost;
        char *localIP;
        char buffer[1024];
    public:
        Client();
        ~Client();
        void Enviar();
        void Recibir();
};

#endif