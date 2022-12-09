#ifndef CORRER_H
#define CORRER_H

#include "Config.h"
#include <Caudal.h>

struct Variables_Correr
{
    bool ejecucion{false};
    bool Valvula_manual{false};
    bool conmutador{false};
    bool array[Nfilas]{};
};

namespace Correr
{
    void setup();
    void loop();
};

#endif