#include <Memoria_no_volatil.h>

short Memoria_no_volatil::ID = 0;

const unsigned int Memoria_no_volatil::size{EEPROM.length()};

Memoria_no_volatil::Memoria_no_volatil() : Function{true}, estado_memoria{false}
{
    ID = EEPROM.read(EEPROM.length() - 1);
    this->calVal_eepromAdress = 0;
    if(EEPROM[0] != 255)
    {
        short int i = 0;
        do
        {
            if (EEPROM[i] == 255) size_dato = i;
            else i++;
        } while (EEPROM[i] == 255);
        if (i == 1 && EEPROM[i + 2] == 255)
        {
            size_dato = EEPROM.read(calVal_eepromAdress);
            Serial.print("Se está reutilizando un Espacio con tamaño: ");
            Serial.println(size_dato);
            estado_memoria = true;
        }
        else
        {
            Serial.print("Recuperación de memoría correcta. ");
            Serial.print("Tamaño: ");
            Serial.println(size_dato);
        }
    }
    else
    {
        Serial.println("Se ha asignado en la posición cero correctamente");
        ID += 1 ;
    }
}

Memoria_no_volatil::~Memoria_no_volatil()
{
    for (int unsigned i = calVal_eepromAdress; i < size_dato; i++)
    {
        if (EEPROM[i] != 255)
            EEPROM[i] = 255;
    }
    EEPROM.write(calVal_eepromAdress, size);
    Serial.println("Dato eliminado correctamente");
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

void Memoria_no_volatil::imprimir()
{
    for(int i=0; i<50;i++)
    {
        Serial.println(EEPROM[i]);
    }
}

void Memoria_no_volatil::limpiar()
{
    for (int unsigned i = 0; i < size; i++)
    {
        if (EEPROM[i] != 255)
            EEPROM[i] = 255;
    }
    Serial.println("Memoria limpiada correctamente");
}

int unsigned& Memoria_no_volatil::operator[](int orden)
{
    ID = EEPROM.read(EEPROM.length() - 1);
    int i{0};
    short contador{0};
    while (contador <= ID + 1 and i < 20)
    {
        if (orden == 0)
        {
            break;
        }
        else if (EEPROM[i] == 255)
        {
            if (contador == orden && orden <= ID && orden != 0)
            {
                this->calVal_eepromAdress = i + 1;
                Serial.println("Recuperación de memoría correcta");
                short int j = i + 1;
                do
                {
                    if (EEPROM[j] == 255)
                        size_dato = j - (i + 1);
                    else
                        j++;
                } while (EEPROM[j] == 255);
                if (j == 1 && EEPROM[i + 2] == 255)
                {
                    size_dato = EEPROM.read(calVal_eepromAdress);
                    Serial.print("Se está reutilizando un Espacio con tamaño: ");
                    Serial.println(size_dato);
                    estado_memoria = true;
                }
                else
                {
                    Serial.print("Recuperación de memoría correcta. ");
                    Serial.print("Con tamaño: ");
                    Serial.println(size_dato);
                }
                break;
            }
            else if (EEPROM[i + 1] == 255 && ID < orden)
            {
                this->calVal_eepromAdress = i + 1;
                Serial.println("Asignación correcta de memoría correcta");
                size_dato = (size-i-1);
                Serial.println("Tamaño disponible:");
                break;
            }
            else if (orden > ID + 1)
            {
                Serial.println("Asignación muy grande, intente con una asignación menor");
                this->calVal_eepromAdress = 0;
                break;
            }
            else if (EEPROM[i + 1] != 255)
                contador++;
        }
        i++;
        Serial.println(i);
    }
    EEPROM[(EEPROM.length() - 1)] = (orden >= ID) ? orden : ID;
    if (orden > ID + 1)
    {
        Serial.println("No se pudo recuperar o Asignar la memoria corectamente");
        Function = false;
    }
    else
    {
        Serial.print("Se almaceno en la dirreción: ");
        Serial.print(calVal_eepromAdress);
        Serial.print(" con el ID: ");
        Serial.println(EEPROM[(EEPROM.length() - 1)]);
    }
    return calVal_eepromAdress;
}