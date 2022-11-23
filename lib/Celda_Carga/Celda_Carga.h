#ifndef CELDA_CARGA_H
#define CELDA_CARGA_H

#include <HX711_ADC.h>

class Celda_Carga: public HX711_ADC
{
    public:
        Celda_Carga();
};

#else
    class Celda Carga{};

#endif