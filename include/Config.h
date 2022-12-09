#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <HX711.h>
#include <Arduino.h>

#define Nfilas 5

namespace Variables
{
    constexpr int VELOCIDAD_DATOS{9600};
};

struct Variables_config
{
    float peso_total{0};
    float radio_llave{0};
    float valores[3]{0};
    float y[Nfilas]{};
    float suma_valores{0};
};

struct Variables_Celda_Carga
{
    const uint8_t HX711_dout{A1};
    const uint8_t HX711_sck{A0};
    unsigned long t{0};
    const int calVal_eepromAdress{0};
    short int Num_elementos{8};
    struct Variables_config Valores{};
    float x[Nfilas]{};
    const float densidad{1000};
};


#endif 