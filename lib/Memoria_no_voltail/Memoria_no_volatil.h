#ifndef MEMORIA_NO_VOLATIL_H
#define MEMORIA_NO_VOLATIL_H

#include <EEPROM.h>
#include <Arduino.h>

//Enumeración de los elementos guardados en memoria
enum class Variables_Memoria
{
    Posicion,           // Posición: 0
    peso_total,        // Posición: 1
    radio_llave,       // Posición: 2
    valores_regresion, // Posición: 3
    suma_valores,      // Posición: 4
    Caudal,            // Posición: 5
    altura,            // Posición: 6
    tara,              // Posición: 7
    dias               // Posición: 8
};

//Elementos de la regresión cuadratica:
struct valores
{
    float x0{1.92}; //Termino independiente.
    float x1{2.3}; //Termino de grado 1.
    float x2{1}; //Termino de grado 2.
};

struct time
{
    unsigned long  hora{0};
    unsigned short dia{30};
};


//Elementos que se almacenan en la EEPROM:
struct Variables_Guardar
{
    long t{210};
    float peso_total{12.3};
    float radio_llave{10.5};
    valores values;
    float suma_valores{1.5};
    float Caudal_total{1.6};
    float altura{1.8};
    time tiempo{};
    float lista_dias[30]{0};
};

#define Num_elementos 7

//Clase que se encarga de la introducción y menejo de los datos almacenados en la EEPROM.
class Memoria_no_volatil
{
    private:
        int unsigned calVal_eepromAdress; //Dirreción en la que se ha almacenado el dato.
        bool Function; //Variable que establece el flujo del programa.
        static short ID; //Identificador númerico del dato.
        static const unsigned int size; //Número de bytes disponibles para escribir.
        int unsigned size_dato; // Tamaño del dato accedido.
    
    protected:
        bool estado_memoria; //Estado del dato accedido.
        Variables_Memoria names; // Nombre del dato al que fue accedido.
        static bool Cambio; //Si se ha modificado algun dato.
        time uso; //Guarda datos sobre los días de uso y el tiempo de utilización.

    public:
        Memoria_no_volatil();
        ~Memoria_no_volatil();
        template <typename T1> bool Escritura_One(T1 &datos)
        {
            if (Function)
            {
                if (names != Variables_Memoria::dias)
                {
                    if (size_dato == sizeof(T1))
                    {
                        EEPROM.put(calVal_eepromAdress, datos);
                        estado_memoria = true;
                        Cambio = true;
                        return true;
                    }
                    else if (size_dato >= 10)
                    {
                        EEPROM.put(calVal_eepromAdress, datos);
                        size_dato = sizeof(T1);
                        estado_memoria = true;
                        Cambio = true;
                        return true;
                    }
                }
                else estado_memoria = false;
            }
            return false;
        }
        short get_Adress();
        Variables_Memoria& indentificar();
        int limpiar(); //Regresa a la dirreción #0
        short &get_id();
        void imprimir();
        bool inicializar_list();
        template <typename T2> bool Lectura(T2 &data) // Función para leer los datos
        {
            if (Function)
            {
                if (size_dato == sizeof(T2) and estado_memoria)
                {
                    EEPROM.get(calVal_eepromAdress, data);
                    return true;
                }
                else
                    return false;
            }
            return false;
        }
        const int unsigned &operator[](short indx);
        int unsigned &lenght();
        void Escritura_estructura(float values[3]);
        bool Escritura_lista(float& value, short tamano);
        bool Lectura_lista(float value[30]);
        const bool &state();
        time &inicializar();
};

#else
    #define CLASE_MEMORIA true

#endif