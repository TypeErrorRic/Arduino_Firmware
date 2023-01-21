#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>
#include "C:\Users\user\Desktop\Taller_de_Ing\Azucarado\.pio\libdeps\uno\LiquidCrystal\src\LiquidCrystal.h"

#define Nfilas 5

//Variables de configuración:
namespace Variables
{
    constexpr int VELOCIDAD_DATOS{9600};
    static LiquidCrystal Lcd(7, 6, 5, 4, 3, 2);
};

//Variables de configuracion de funcionamiento:

struct Variables_config
{
    const uint8_t HX711_dout{A1};
    const uint8_t HX711_sck{A0};
    const short calVal_eepromAdress{0};
    const float densidad{1000};
};

#endif 