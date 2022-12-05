#ifndef MEMORIA_NO_VOLATIL_H
#define MEMORIA_NO_VOLATIL_H

#include <EEPROM.h>
#include <Arduino.h>

class Memoria_no_volatil
{
    private:
        int unsigned calVal_eepromAdress;
        bool Function;
        static short ID;
        static const unsigned int size;
        short unsigned size_dato;
    
    protected:
        bool estado_memoria;

    public:
        Memoria_no_volatil();
        ~Memoria_no_volatil();
        template <typename T1> void Escritura(T1& datos)
        {
            if (Function)
            {
                if (size_dato == sizeof(T1))
                {
                    Serial.println("Guardando datos");
                    EEPROM.put(calVal_eepromAdress, datos);
                    estado_memoria = true;
                }
                else if(size_dato >= 50)
                {
                    EEPROM.put(calVal_eepromAdress, datos);
                    size_dato = sizeof(T1);
                    estado_memoria = true;
                }
                else
                    Serial.println("Erro en la escritura: Memoria mal asignada.");
            }
            else Serial.println("Erro en la escritura: ID no correcto");
        }
        template <typename T2> T2 Lectura(T2& data)
        {
            if (size_dato == sizeof(T2) and estado_memoria)
            {
                Serial.print("Obteniendo dato: ");
                Serial.println(EEPROM.get(calVal_eepromAdress, data));
                return EEPROM.get(calVal_eepromAdress, data);
            }
            else
            {
                Serial.println("Error en la lectura de datos");
                return EEPROM.get(calVal_eepromAdress, data);
            }
        }
        short get_Adress();
        const bool & state();
        short &indentificar();
        void imprimir();
        static void limpiar();
        const int unsigned &operator[](int indx);
        short unsigned &lenght();
        template <typename T3> void reemplazar_dato(T3 data)
        {
            if (unsigned(ID) < calVal_eepromAdress and (((size_dato) > (sizeof(T3)+3)) and not(estado_memoria) and size_dato < 50))
            {
                Serial.println("Guardando datos");
                EEPROM.put(calVal_eepromAdress, data);
                EEPROM.write((calVal_eepromAdress + sizeof(T3) + 1), (size_dato - sizeof(T3)));
                size_dato = sizeof(T3);
                estado_memoria = true;
                EEPROM.write(calVal_eepromAdress, size_dato);
                Serial.println("Dato reescrito correctamente");
                estado_memoria = false;
            }
            else Serial.println("Memoria insuficiente para reescribrir");
        }
};
#else
    #define INSTANCIADO "Memoria ya usada"
#endif