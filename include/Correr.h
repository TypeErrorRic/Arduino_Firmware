#ifndef CORRER_H
#define CORRER_H

#include "Config.h"
#include <Caudal.h>

enum class sensores
{
    Sensor1 = 2,
    Sensor2 = 3,
    Sensor3 = 4,
    Sensor4 = 5,
    Sensor5 = 6,
    comutador = 7,
    Eletrovalvula = 8,
    Alerta_visual = 9
};

struct Variables_Correr
{
    float data{};
    float peso{};
    int apuntador{};
    bool ejecucion{true};
    short int Num_elementos{8};
    bool Valvula_manual{false};
    const float densidad{1000};
    const float radio{2.5};
};

namespace Correr
{
    bool disminucion{false};
    void Sensor_estado_6(sensores sensor);
    void setup();
    void loop();
};

#endif