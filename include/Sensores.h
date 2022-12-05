#ifndef SENSOR_H
#define SENSOR_H

#include "Config.h"
#include <Caudal.h>

struct Variables_Correr2
{
    float data{};
    int apuntador{};
    bool ejecucion{false};
    bool Valvula_manual{false};
};

namespace Correr2
{
    char Sensor_estado_6();
    void setup();
    void loop();
};

#endif