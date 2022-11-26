/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   -------------------------------------------------------------------------------------
*/

#ifndef CELDA_CARGA_H
#define CELDA_CARGA_H


#include <HX711_ADC.h>
#include "config.h"

//Clase Celda de carga:
class Celda_Carga
{
    private:
        HX711_ADC Balanza;
        unsigned long stabilizingtime;
        bool _tare;
        boolean resume;
        Memoria_no_volatil calibracion;

    protected:
        void Calibracion_peso();
        void Configuracion_tare();
        void changeSavedCalFactor();

    public: 
        Celda_Carga(const short Port_conect, const short Port_out, bool _tare);
        void Update();
};

#else
    class Celda Carga{};

#endif