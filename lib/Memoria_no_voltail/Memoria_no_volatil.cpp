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
    this->names = Variables_Memoria::tara;
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
    while (contador <= ID and i < 50)
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
            this->names = Variables_Memoria::tara;
        else if (orden == static_cast<short>(Variables_Memoria::peso_total))
            names = Variables_Memoria::peso_total;
        else if (orden == static_cast<short>(Variables_Memoria::radio_llave))
            names = Variables_Memoria::radio_llave;
        else if (orden == static_cast<short>(Variables_Memoria::valores_regresion))
            names = Variables_Memoria::valores_regresion;
        else if (orden == static_cast<short>(Variables_Memoria::suma_valores))
            names = Variables_Memoria::suma_valores;
    }
    return size_dato;
}

int unsigned &Memoria_no_volatil::lenght()
{
    return size_dato;
}

short Memoria_no_volatil::set_up(Variables_Guardar &variables)
{
    if (Function && ID == Num_elementos)
    {
        if(this[0].Lectura(variables.t))
            if(this[1].Lectura(variables.peso_total))
                if(this[2].Lectura(variables.radio_llave))
                    if(this[3].Lectura(variables.values))
                        if(this[4].Lectura(variables.suma_valores))
                            if (this[5].Lectura(variables.Caudal_total))
                                if (this[6].Lectura(variables.altura))
                                {
                                    this[8].Lectura(uso);
                                    if (this[7].Lectura_lista(variables.lista_dias))
                                    {
                                        if (Cambio)
                                        {
                                            Serial.println("Cargado");
                                            return 0;
                                        }
                                    }
                                }
    }
    return -1;
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
    for (int i = 0; i < 50; i++)
    {
        Serial.println(EEPROM[i]);
    }
    Serial.println(EEPROM[1023]);
}

const bool &Memoria_no_volatil::state()
{
    return estado_memoria;
}

void Memoria_no_volatil::Escritura_lista(float& value)
{
    EEPROM.put((calVal_eepromAdress + uso.dia*sizeof(value)), value);
}
bool Memoria_no_volatil::Lectura_lista(float value[30])
{
    for(short i = 0; i < 30; i++)
    {
        EEPROM.get(calVal_eepromAdress + (i*sizeof(value[i])), value[i]);
    }
    if(value[uso.dia] != 0)
        return true;
    return false;
}