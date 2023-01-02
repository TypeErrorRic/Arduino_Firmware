#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <Caudal.h>

enum estados
{
    DESACTIVADO,//Estado desactivado del sensor.
    ESTADO20,   //Se activo el sensor de 20%
    ESTADO40,   //Se acivo el sensor de 40%
    ESTADO60,   //Se activo el sensor de 60%
    ESTADO80,   //Se activo el sensor de 80%
    ESTADO90    //Se activo el sensor de 90%
};

struct sensores
{
    const short ELECTROVALVULA{13};
    const short VALVULA_MANUAL{12};
    const short ALARMA{11};
    const short CONUMTADOR{10};
};

class Functional
{
    private:
        Caudal &caudal;
        bool& ejecucion; //Está variable define si se ejectua el llenado.
        bool& Valvula_manual; //Está variable define si la valvula manual está abierto o no.
        bool &conmutador; // Está variable define si el conmutador está encendido o no.
        char aux; //Está variables se usa para capturar los datos.
        bool pass_f; //Está variable se usa para pasar del estado 5 al estado 4. 
        bool exceso; //Está variable se usa para pasar del estado 4 al estado 5;
        static sensores sensor;

    protected:
        bool estado_90(bool array[5]);
        char &Sensores_estado_6(bool array[5], bool descender);
        bool estado_20(bool array[5]);
        //void Regresion(Regresion_Cuadratica &Cuadratica);

    public:
        Functional(Caudal& caudal, bool &ejecucion, bool &Valvula, bool& conmutador) : 
            caudal(caudal), ejecucion{ejecucion}, Valvula_manual{Valvula}, conmutador{conmutador}, 
            aux{'\0'}, pass_f{false}, exceso{false}{}
        void init(float array[3], float &altura);
        void vericacion_vaciado(bool array[5]);
        char& abrir_valvula();
        bool cerrar_valvula(bool array[5]);
        bool terminar_proceso(bool array[5], char& aux);
        void Encender(bool array[5]);
        void llenado_nivel(bool array[5], bool descender);
};


#else 
    class Functional{};

    
#endif