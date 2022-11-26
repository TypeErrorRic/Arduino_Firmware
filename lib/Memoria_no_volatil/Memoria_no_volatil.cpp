#include <Memoria_no_volatil.h>

short Memoria_no_volatil::ID = int(EEPROM.read(EEPROM.length() - 1));

Memoria_no_volatil::Memoria_no_volatil(const short orden) : Function{true}, size{EEPROM.length()}
{
    int contador{0}, i{0};
    while(contador <= ID+1 and i<1023)
    {
        if (EEPROM[i] == 255)
        {
            if(contador == orden && orden <= ID && orden != 0)
            {
                this->calVal_eepromAdress = i+1;
                Serial.println("Asignación correcta y recuperación de memoría correcta");
                break;
            }
            else if(orden == 0)
            {
                Serial.println("Se ha almacenado en la posición cero correctamente");
                this->calVal_eepromAdress = 0;
                break;
            }
            else if (EEPROM[i + 1] == 255 && ID < orden)
            {
                this->calVal_eepromAdress = i + 1;
                Serial.println("Asignación correcta de memoría correcta");
                break;
            }
            else if(orden > ID+1)
            {
                Serial.println("Asignación muy grande, intente con una asignación menor");
                this->calVal_eepromAdress = 0;
                break;
            }
            else if (EEPROM[i + 1] != 255)
            {
                contador++;
                continue;
            }
        }
        i++;
        Serial.println(i);
    }
    EEPROM[(EEPROM.length() - 1)] = (orden >= ID) ? orden : ID;
    if(orden > ID+1)
    {
        Serial.println("No se pudo recuperar o Asignar la memoria corectamente");
        Function = false;
    }
    else
    {
        Serial.print("Se almaceno en: ");
        Serial.print(calVal_eepromAdress);
        Serial.print(" con el ID: ");
        Serial.println(ID);
    }
}

Memoria_no_volatil::~Memoria_no_volatil()
{
    for(int unsigned i = 0; i < size; i++)
    {
        if(EEPROM[i] != 255) EEPROM[i] = 255; 
    }
    Serial.println("Memoria limpiada correctamente");
}

short Memoria_no_volatil::get_Adress()
{
    if(Function)return calVal_eepromAdress;
    else return 0;
}

const bool &Memoria_no_volatil::function()
{
    return Function;
}

short & Memoria_no_volatil::indentificar()
{
    Serial.println(ID);
    return ID;
}
