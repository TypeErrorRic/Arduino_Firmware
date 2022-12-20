#include <iostream>
#include <winsock2.h>

class Client
{
    private:
        WSADATA WSAData;
        SOCKET server;
        SOCKADDR_IN addr;
        char buffer[1024];
    public:
        Client();
        ~Client();
        void Enviar();
        void Recibir();
};
