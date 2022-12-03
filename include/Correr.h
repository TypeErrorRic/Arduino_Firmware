#ifndef CORRER_H
#define CORRER_H

#include "Config.h"
#include <Caudal.h>

struct Variables_Correr
{
    float data{};
    float peso{};
    int apuntador{};
    bool ejecucion{false};
    bool Valvula_manual{false};
    const float densidad{1000};
};

namespace Correr
{
    void Sensor_estado_6(sensores sensor);
    void setup();
    void loop();
};

#endif