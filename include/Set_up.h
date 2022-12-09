#ifndef SET_UP_H
#define SET_UP_H

#include "Config.h"
#include <Regresion_Cuadratica.h>

#define max_value 10

enum class _sensor_
{
    Sensor0 = 1,
    Sensor1 = 2,
    Sensor2 = 3,
    Sensor3 = 4,
    Sensor4 = 5,
    Sensor5 = 6,
    comutador = 7,
    Eletrovalvula = 13,
    Alerta_visual = 12
};

typedef enum _sensor_ sensor;

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
