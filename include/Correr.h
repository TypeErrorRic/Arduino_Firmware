#ifndef CORRER_H
#define CORRER_H

#include "Lecture_data.h"
#include "Manejo_datos.h"
#include <Caudal.h>
#include <Memoria_no_volatil.h>

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
    void calibracion(bool& validdacion);
};

#endif