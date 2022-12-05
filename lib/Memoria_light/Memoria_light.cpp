#include <Memorial_light.h>

const unsigned int Memoria_light::size{EEPROM.length()};

Memoria_light::Memoria_light() : estado_memoria{false}
{
    this->calVal_eepromAdress = 0;
    if (EEPROM[calVal_eepromAdress] != 255)
    {
        short int i = 0;
        do
        {
            i++;
            if (EEPROM[i] == 255)
            {
                size_dato = i;
            }
            Serial.println(i);
        } while (EEPROM[i] != 255);
        Serial.print("Recuperación de memoría correcta. ");
        Serial.print("Tamaño: ");
        Serial.println(size_dato);
        estado_memoria = true;
    }
    else
    {
        Serial.println("Se ha asignado en la posición cero correctamente");
    }
}
Memoria_light::~Memoria_light()
{
    Serial.println("Datos almacenados correctamente.");
}


const bool &Memoria_light::state()
{
    return estado_memoria;
}

void Memoria_light::imprimir()
{
    for (int i = 0; i < 50; i++)
    {
        Serial.println(EEPROM[i]);
    }
    Serial.println(EEPROM[1023]);
}

void Memoria_light::limpiar()
{
    for (int unsigned i = 0; i < size; i++)
    {
        if (EEPROM[i] != 255)
            EEPROM[i] = 255;
    }
    Serial.println("Memoria limpiada correctamente");
}