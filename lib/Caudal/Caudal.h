#ifndef CAUDAL_H
#define CAUDAL_H

#define elementos 3

class Caudal
{
    private:
        const float densidad;
        const float radio;
        float derivada[elementos];
        const float *valores;
        static const float Diametro;
        static const float Gravedad;
    public:
        Caudal(const float densidad, const float radio);
        ~Caudal();
        float posicion(float &peso);
        float volumen(float &peso);
        void set_valores(float valores[3]);
};

#else
    class Operaciones_matematicas;

#endif