#ifndef CAUDAL_H
#define CAUDAL_H

#include <Arduino.h>

#define elementos 3

class Caudal
{
    private:
        const float densidad;
        float velocidad[elementos];
        const float *valores;
        float Peso_ultimo;
        const float Peso_total;
        unsigned long tiempo;
        float velocidad_caudal;
        float volumen_ultimo;
        const float radio;
    protected:
        float volumen(float &peso);
        bool variacion_peso(float volumen);
    public:
        Caudal(const float densidad, const float peso, const float& radio);
        void set_valores(float valores[3], float velocidad);
        void Valores(float &peso);
        float estado(float &peso);
};

#else
    class Operaciones_matematicas;

#endif