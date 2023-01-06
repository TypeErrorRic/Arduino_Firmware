/**
 * @file Testeo.cpp
 * @author Ricardo Pabón Serna (ricardo.pabon@correounivalle.edu.co)
 * @brief Implementación de Testeo.h
 * @version 0.1
 * @date 2022-10-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "..\include\Lecture_data.h"

// Variables globales de las funciones:
static int dataReal{0}, dataDecimal{0}, dataPow{1};
static bool isDecimalStage{false}, validacion{true}, validacion1{true}, validacion2{true};
static unsigned short contador{0};
static bool siguiente{true};
float data{0};
/**
 * @brief Ambito de Teseto.
 */
namespace Lectura_data
{
    /**
     * @brief Se encarga de recivir los números enteros
     *
     * @return float
     */
    void Lectura_events_numerics(char* array, short& size)
    {
        const unsigned short int Size{6};
        short count{0};
        char aux{};
        while(count <= size) {
            if(count < size)
                aux = array[count];
            // Captura los datos siempre y cuando no pase los caracteres guardados y no sea el caracter un salto de linea.
            if (count < size && contador < Size - 1)
            {
                count++;
                // Se encagara de detectar si es un número decimal.
                if ((aux == '.' || aux == ',') && validacion1 && contador != 0)
                {
                    isDecimalStage = true;
                    validacion1 = false;
                    contador++;
                }
                // Se encarga de revisar y confirmar si es un valor númerico. Si lo es entonces es guardado
                else if (aux >= '0' && aux <= '9')
                {
                    if (isDecimalStage == false)
                    {
                        dataReal = (dataReal * 10) + (aux - '0');
                    }
                    else
                    {
                        dataDecimal = (dataDecimal * 10) + (aux - '0');
                        dataPow *= 10;
                    }
                    contador++;
                }
                // Se encarga de mostrar por pantalla los caracteres no validos.
                else
                {
                    data = 0;
                    validacion = false;
                }
            }
            // Se encarga de imprimir el número escrito.
            else if ((size = count || contador == Size) && validacion)
            {
                data = (float)dataReal + (float)dataDecimal / dataPow;
                dataReal = 0;
                dataDecimal = 0;
                dataPow = 1;
                isDecimalStage = false;
                contador = 0;
                validacion1 = true;
                validacion2 = true;
                siguiente = false;
                break;
            }
            // Se encarga de indicar que se ha incluido valores no númericos; por lo que, ignora la toma de datos.
            else if (not(validacion) && (size = count || contador == Size))
            {
                dataReal = 0;
                dataDecimal = 0;
                dataPow = 1;
                isDecimalStage = false;
                contador = 0;
                validacion1 = true;
                validacion2 = true;
                validacion = true;
                break;
            }
        }
    }

    char *Asignacion_strings(short& size)
    {
        if (Serial.available())
        {
            String datos = Serial.readString();
            if(datos.length() > 3)
            {
                char *variable = new char[datos.length()];
                datos.toCharArray(variable, (datos.length()+1));
                size = datos.length();
                return variable;
            }
        }
        return nullptr;
    }

    String* Captura_eventos(short& size)
    {
        char * list = nullptr;
        short cantidad{1};
        short Values[8] = {0,0,0,0,0,0,0,0};
        Values[0] = 0;
        while(list == nullptr)
            list = Asignacion_strings(size);
        char variable = list[size - 2];
        for(short i = (Values[0] + 1); i< size - 2 ; i++)
        {
            if(list[i] == '/' && cantidad <= 5)
            {
                Values[cantidad] = i;
                cantidad++;
            }
        }
        String *lista = new String [cantidad+1];
        lista[0] = variable;
        short indice = 0;
        while(Values[indice] != size - 2 && cantidad != 1)
        {
            String aux = "";
            if(Values[indice + 1] == 0)
            {
                Values[indice + 1] = size - 2;
            }
            for(short i = (Values[indice] + 1); i < Values[indice + 1]; i++)
            {
                aux += list[i];
            }
            lista[indice + 1] = aux;
            indice++;
        }
        delete[] list;
        size = cantidad;
        return lista;
    }

    float Convertir_a_Numero(String numero)
    {
        short size = numero.length();
        siguiente = true;
        char *array = new char[size+1];
        numero.toCharArray(array, (size+1));
        while(siguiente)
        {
            data = 0;
            Lectura_events_numerics(array, size);
        }
        delete[] array;
        return data;
    }
}

/**
 * @note Implementación de las funciones de la manera más optima posible sin recurrir al uso de librerías.
 *
 * @attention Este es la implementación de funciones de testeo para comprobar el funcionamiento
 *            de la regresión de Matriz.
 */