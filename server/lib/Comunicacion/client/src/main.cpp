#include "..\include\Client.h"

int wmain()
{
    Client *Cliente = new Client();
    while (true)
    {
        Cliente->Recibir();
    }
    delete Cliente;
    return 0;
}