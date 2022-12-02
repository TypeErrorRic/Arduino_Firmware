#ifndef SET_UP_H
#define SET_UP_H

#include "Config.h"
#include <Regresion_Cuadratica.h>


struct Variables_Calibracion
{
    const short Calculo_Peso{10};
    float suma_valores{};
    float peso{0};
};

namespace Calibracion
{
    void setup();
    void loop();
};

#endif 
