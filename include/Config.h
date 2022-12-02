#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <HX711.h>
#include <Memoria_no_volatil.h>
#include <Arduino.h>
#include "Testeo.h"

namespace Variables
{
    constexpr int VELOCIDAD_DATOS{9600};
    const int Nfilas = 5;   // Precisión de la medida tomada.
    struct Valores_Sensores // Valores Para el calculo de la matriz.
    {
        float x[Nfilas]{}; // Medidas de la masa con respecto al eje X.
        float y[Nfilas]{}; // Medidas de la masa con respecto al eje Y.
    };
};

struct Variables_Celda_Carga
{
    const uint8_t HX711_dout{A1};
    const uint8_t HX711_sck{A0};
    unsigned long t{0};
    const int calVal_eepromAdress{0};
    unsigned long stabilizingtime{2000}; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
    boolean _tare{true};  // set this to false if you don't want tare to be performed in the next step
};


#endif 