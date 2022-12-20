#include "..\client\include\Client.h"

Client::Client()
{
    std::cout << "Conectando al servidor..." << std::endl;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_addr.s_addr = inet_addr("192.168.0.8");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    std::cout << "Conectado al Servidor!" << std::endl;
}

Client::~Client()
{
    closesocket(server);
    WSACleanup();
    std::cout << "Socket cerrado." << std::endl;
}

void Client::Enviar()
{
    std::cout << "Escribe el mensaje a enviar: ";
    std::cin >> this->buffer;
    send(server, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    std::cout << "Mensaje enviado!" << std::endl;
}

void Client::Recibir()
{
    recv(server, buffer, sizeof(buffer), 0);
    std::cout << "El servidor dice: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));
}
