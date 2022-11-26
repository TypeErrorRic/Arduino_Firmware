#ifndef MEMORIA_NO_VOLATIL_H
#define MEMORIA_NO_VOLATIL_H

#include <EEPROM.h>
#include <Arduino.h>

#undef MEMORIA_INICIAL

class Memoria_no_volatil
{
    private:
        short calVal_eepromAdress;
        bool Function;
        static short ID;
        unsigned int size;

    public:
        Memoria_no_volatil(const short Adress);
        ~Memoria_no_volatil();
        template <typename T1> void Escritura(T1 datos)
        {
            if(Function)
            {
                Serial.println("Guardando datos");
                EEPROM.put(calVal_eepromAdress, datos);
            }
            else Serial.println("Erro en la escritura");
        }
        template <typename T2> T2 Lectura(T2 data)
        {
            Serial.print("Obteniendo dato: ");
            Serial.println(EEPROM.get(calVal_eepromAdress, data));
            return EEPROM.get(calVal_eepromAdress, data);
        }
        short get_Adress();
        const bool &function();
        short &indentificar();
        void imprimir();
};
#else
    #define Pass false
#endif