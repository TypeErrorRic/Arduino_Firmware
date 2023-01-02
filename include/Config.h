#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>

#define Nfilas 5

//Variables de configuración:
namespace Variables
{
    constexpr int VELOCIDAD_DATOS{9600};
};

//Variables de configuracion de funcionamiento:

struct Variables_config
{
    const uint8_t HX711_dout{A1};
    const uint8_t HX711_sck{A0};
    const short calVal_eepromAdress{0};
    const short Num_elementos{8};
    const float densidad{1000};
};

#endif 