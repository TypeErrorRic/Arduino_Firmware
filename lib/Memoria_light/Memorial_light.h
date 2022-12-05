#ifndef MEMORIA_LIGHT_H
#define MEMORIA_LIGHT_H

#include <EEPROM.h>
#include <Arduino.h>

class Memoria_light
{
private:
    int unsigned calVal_eepromAdress;
    static const unsigned int size;
    short unsigned size_dato;

protected:
    bool estado_memoria;

public:
    Memoria_light();
    ~Memoria_light();
    template <typename T1> void Escritura(T1 &datos)
    {
        if (size_dato == sizeof(T1))
        {
            Serial.println("Guardando datos");
            EEPROM.put(calVal_eepromAdress, datos);
            estado_memoria = true;
        }
        else if (size_dato >= 50)
        {
            EEPROM.put(calVal_eepromAdress, datos);
            size_dato = sizeof(T1);
            estado_memoria = true;
        }
        else
            Serial.println("Erro en la escritura: Memoria mal asignada.");
    }
    template <typename T2> void Lectura(T2 &data)
    {
        if (size_dato == sizeof(T2) and estado_memoria)
        {
            Serial.print("Obteniendo dato: ");
            Serial.println(EEPROM.get(calVal_eepromAdress, data));
            EEPROM.get(calVal_eepromAdress, data);
        }
        else
        {
            Serial.println("Error en la lectura de datos");
        }
    }
    const bool &state();
    void imprimir();
    static void limpiar();
    short unsigned &lenght();
};
#else
#define INSTANCIADO "Memoria ya usada"
#endif