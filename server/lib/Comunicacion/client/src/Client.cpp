#include "..\include\Client.h"

Client::Client(): buffer{0}
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"Inicialización fallida: %d\n", iResult);
    }
    //----------------------
    // Create a SOCKET for connecting to server
    SOCKET ConnectSocket;
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET)
    {
        wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(27015);

    //----------------------
    // Connect to server.
    iResult = connect(ConnectSocket, (SOCKADDR *)&clientService, sizeof(clientService));
    if (iResult == SOCKET_ERROR)
    {
        wprintf(L"Sucedio un error en la conexión: %ld\n", WSAGetLastError());
        iResult = closesocket(ConnectSocket);
        if (iResult == SOCKET_ERROR)
            wprintf(L"ERROR: %ld\n", WSAGetLastError());
        WSACleanup();
    }

    wprintf(L"Connected to server.\n");

    iResult = closesocket(ConnectSocket);
    if (iResult == SOCKET_ERROR)
    {
        wprintf(L"ERROR DE CONEXIÓN FINALIZADA: %ld\n", WSAGetLastError());
    }
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
