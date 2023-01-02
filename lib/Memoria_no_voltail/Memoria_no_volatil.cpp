#include <Memoria_no_volatil.h>

// Identificador:
short Memoria_no_volatil::ID = 0;

// Número de bytes disponibles.
const unsigned int Memoria_no_volatil::size{EEPROM.length()};

// Verfica si algún dato se ha cambiado.
bool Memoria_no_volatil::Cambio{false};

Memoria_no_volatil::Memoria_no_volatil() : Function{true}, estado_memoria{false}
{
    ID = (EEPROM.read(EEPROM.length() - 1) != 255) ? EEPROM.read(EEPROM.length() - 1) : 0;
    this->calVal_eepromAdress = 0;
    this->names = Variables_Memoria::Posicion;
    if (EEPROM[calVal_eepromAdress] != 255)
    {
        short int i = 0;
        while (EEPROM[i] != 255)
            i++;
        size_dato = i;
        estado_memoria = true;
    }
    else
    {
        estado_memoria = false;
        size_dato = size - 1;
    }
    if (EEPROM[size - 1] == 255)
        EEPROM[size - 1] = 1;
    Serial.println(size_dato);
}

Memoria_no_volatil::~Memoria_no_volatil()
{
    Function = false;
    Cambio = false;
    //imprimir();
}

short Memoria_no_volatil::get_Adress()
{
    if (Function)
        return calVal_eepromAdress;
    else
        return -1;
}

Variables_Memoria &Memoria_no_volatil::indentificar()
{
    return names;
}

int Memoria_no_volatil::limpiar()
{
    int aux{EEPROM[size - 1]};
    for (int unsigned i = 0; i < size; i++)
    {
        if (EEPROM[i] != 255)
            EEPROM[i] = 255;
    }
    this->calVal_eepromAdress = 0;
    estado_memoria = false;
    EEPROM[size - 1] = 1;
    size_dato = size - 1;
    return aux;
}

const int unsigned &Memoria_no_volatil::operator[](short orden)
{
    ID = EEPROM.read(EEPROM.length() - 1);
    int i{0};
    short contador{1};
    while (contador <= ID)
    {
        //Asignación en la posición index = 0;
        if (orden == 0 || EEPROM[0] == 255)
        {
            this->calVal_eepromAdress = 0;
            while (EEPROM[i] != 255)
                i++;
            if (size_dato == 0)
            {
                size_dato = size;
                estado_memoria = false;
            }
            else
            {
                size_dato = i;
                estado_memoria = true;
            }
            break;
        }
        else if (orden != 0 && EEPROM[i] == 255 && (orden + 1) <= ID + short(1))
        {
            //Asignación en una posición idex < ID.      
            if (contador == orden && (orden + 1) <= ID)
            {
                this->calVal_eepromAdress = i + 1;
                short int j = i + 1;
                while (EEPROM[j] != 255)
                    j++;
                if (j != 0)
                    size_dato = j - (i + 1);
                else
                {
                    estado_memoria = false;
                    Function = false;
                    this->limpiar();
                    Serial.println("Error");
                    break;
                }
                break;
            }
            //Asignación nueva en la posición index = ID+1
            else if (EEPROM[i + 1] == 255 && ID < (orden + 1) && contador == ID)
            {
                this->calVal_eepromAdress = i + 1;
                size_dato = (size - i - 1);
                estado_memoria = false;
                EEPROM[size - 1] = orden + 1;
                break;
            }
            //Asignación muy grande. ERROR.
            else if ((orden + 1) > ID + 1)
            {
                size_dato = 0;
                this->calVal_eepromAdress = 0;
                break;
            }
            //Identificador de datos.
            else if (EEPROM[i + 1] != 255)
                contador++;
        }
        i++;
    }
    if ((orden + 1) > ID + 1)
        Function = false;
    else
    {
        // Asignación de nombres de la variables.
        if (orden == static_cast<short>(Variables_Memoria::tara))
        {
            names = Variables_Memoria::tara;
            Serial.println(calVal_eepromAdress);
        }
        else if (orden == static_cast<short>(Variables_Memoria::peso_total))
            names = Variables_Memoria::peso_total;
        else if (orden == static_cast<short>(Variables_Memoria::radio_llave))
            names = Variables_Memoria::radio_llave;
        else if (orden == static_cast<short>(Variables_Memoria::valores_regresion))
            names = Variables_Memoria::valores_regresion;
        else if (orden == static_cast<short>(Variables_Memoria::suma_valores))
            names = Variables_Memoria::suma_valores;
        else if (orden == static_cast<short>(Variables_Memoria::Posicion))
            names = Variables_Memoria::Posicion;
        else if (orden == static_cast<short>(Variables_Memoria::dias))
            names = Variables_Memoria::dias;
        else if (orden == static_cast<short>(Variables_Memoria::altura))
            names = Variables_Memoria::altura;
    }
    return size_dato;
}

int unsigned &Memoria_no_volatil::lenght()
{
    return size_dato;
}

void Memoria_no_volatil::Escritura_estructura(float values[3])
{
    if (Function)
    {
        valores Values;
        Values.x0 = values[0];
        Values.x1 = values[1];
        Values.x2 = values[2];
        EEPROM.put(calVal_eepromAdress, Values);
        estado_memoria = true;
        Cambio = true;
    }
}

void Memoria_no_volatil::imprimir()
{
    for (int i = 0; i < 200; i++)
    {
        Serial.println(EEPROM[i]);
    }
    Serial.println(EEPROM[1023]);
}

const bool &Memoria_no_volatil::state()
{
    return estado_memoria;
}

bool Memoria_no_volatil::Escritura_lista(float& value, short tamano)
{
    if(size >= sizeof(value)*tamano && names == Variables_Memoria::dias)
    {
        if (uso.dia != 0)
            EEPROM.put((calVal_eepromAdress + (uso.dia * sizeof(value)) + 1), value);
        else
            EEPROM.put(calVal_eepromAdress, value);
        estado_memoria = true;
        return true;
    }
    else
        return false;
}

bool Memoria_no_volatil::Lectura_lista(float value[30])
{
    if (EEPROM[calVal_eepromAdress + (30 * sizeof(value[0]))] != 255 && estado_memoria)
    {
        EEPROM.get(calVal_eepromAdress, value[0]);
        for (short i = 1; i < 30; i++)
        {
            EEPROM.get(calVal_eepromAdress + (i * sizeof(value[i]) + 1), value[i]);
        }
        return true;
    }
    return false;
}

time &Memoria_no_volatil::inicializar()
{
    if(size_dato <= (size - 1) && calVal_eepromAdress == 0)
        this -> Lectura(uso);
    return uso;
}

short &Memoria_no_volatil::get_id()
{
    return ID;
}

bool Memoria_no_volatil::inicializar_list()
{
    float aux{10};
    if(size_dato >= 150 && names == Variables_Memoria::dias)
    {
        EEPROM.put(calVal_eepromAdress, aux);
        for (short i = 1; i < 30; i++)
        {
            EEPROM.put((calVal_eepromAdress + (i * sizeof(aux)) + 1), aux);
        }
        estado_memoria = true;
        return true;
    }
    return false;
}
