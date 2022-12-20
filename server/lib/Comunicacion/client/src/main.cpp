#include "client\include\Client.h"

int main()
{
    Client *Cliente = new Client();
    while (true)
    {
        Cliente->Enviar();
        Cliente->Recibir();
    }
    delete Cliente;
    return 0;
}