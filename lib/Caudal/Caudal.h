#ifndef CAUDAL_H
#define CAUDAL_H

#include "C:\Users\user\Desktop\Taller_de_Ing\Azucarado\include\Config.h"
#include <Memoria_no_volatil.h>
#include <HX711.h>

#define elementos 3
#define pi 3.14
#define GRAVEDAD 9.8

class Caudal
{
    private:
        const float densidad; //Valor de la densidad del liquido en ciruculación
        const float *valores; // Valores de la regresión cuadratica o lineal
        const float Peso_total; //Capacidad total de almacenamiento del tanque.
        unsigned long tiempo; //Semilla para trabajar con el tiempo de ejecución.
        float Peso_ultimo;
        float volumen_ultimo; //Ultimo registro del volument.
        float caudal; //Caudal
        const float value_caudal; //Registro del caudal obtenido en buenas condiciones. Calibración
        float tanq_altura; //Altura total del tanque.
        Variables_Guardar& variables; //Estructura de la configruación.
        static HX711 balanza; //Conversor ADC de la Celdad de carga.
        bool advertencia;

    protected:
        float volumen(float &peso);
        bool variacion_peso(float& caudal);
        float& delta_caudal();
        float altura(float &peso);
        bool variacion_altura(float altura, float porcentaje);
        int estado(float &peso);
        float Cal_peso();

    public:
        Caudal(const float& densidad, const float &peso, Variables_Guardar &variables, const float &teoric_caudal);
        void set_valores(float valores[3], float& altura);
        bool& Valores();
        static bool calibracion_escala(long &offset, float &sum, bool& realizar);
};

#endif