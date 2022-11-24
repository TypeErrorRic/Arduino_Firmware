/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   -------------------------------------------------------------------------------------
*/

#ifndef CELDA_CARGA_H
#define CELDA_CARGA_H


#include <HX711_ADC.h>

//Clase Celda de carga:
class Celda_Carga
{
    private:
        HX711_ADC Balanza;
        const int calVal_eepromAdress;
    public:
        Celda_Carga(const short Port_conect, const short Port_out);
        void Calibracion();
};

#else
    class Celda Carga{};

#endif