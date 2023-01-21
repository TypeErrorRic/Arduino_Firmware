#ifndef CAUDAL_H
#define CAUDAL_H

#include "C:\Users\user\Desktop\Taller_de_Ing\Azucarado\include\Config.h"
#include <Memoria_no_volatil.h>
#include <HX711.h>

#define elementos 3
#define pi 3.14
#define GRAVEDAD 9.8

//Sensores:
enum sensores_nivel
{
    Sensor20 = 9,
    Sensor40 = A2,
    Sensor60 = A3,
    Sensor80 = A4,
    Sensor100 = A5,
};

class Caudal
{
    private:
        const float densidad; //Valor de la densidad del liquido en ciruculación
        const float *valores; // Valores de la regresión cuadratica o lineal
        const float Peso_total; //Capacidad total de almacenamiento del tanque.
        unsigned long tiempo; //Semilla para trabajar con el tiempo de ejecución.
        float Peso_ultimo;
        float volumen_ultimo; //Ultimo registro del volumen.
        float caudal; //Caudal
        const float value_caudal; //Registro del caudal obtenido en buenas condiciones. Calibración
        float tanq_altura; //Altura total del tanque.
        Variables_Guardar& variables; //Estructura de la configruación.
        static HX711 balanza; //Conversor ADC de la Celdad de carga.
        bool advertencia;
        float peso_turno; //Cantidad de jugo movido durante el turno.
        LiquidCrystal LCD; //Imprimer valores por medio de una pantalla.
        char nivel; //Se encarga de obtener el nivel en el que está.
        bool confirmacion; //Toma de valores.

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
        int lectura_values();
        char return_value_nivel();
        void Calibracion_niveles(float array[5]);
};

#endif