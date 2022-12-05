#ifndef SET_UP_H
#define SET_UP_H

#include "Config.h"
#include <Regresion_Cuadratica.h>

#define max_value 10

struct Variables_Calibracion
{
    const short Calculo_Peso{10};
    float peso{0};
    unsigned long time{0};
    float velocidad{0};
    float volumen{0};
    float array[max_value];
};

namespace Calibracion
{
    void setup();
    void loop();
};

#endif 
