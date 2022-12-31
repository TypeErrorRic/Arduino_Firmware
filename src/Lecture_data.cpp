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
    int Lectura_events_numerics(float &data)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        const unsigned short int Size{6};
        while (Serial.available() > 0)
        {
            digitalWrite(LED_BUILTIN, LOW);
            char aux = (char)Serial.read();
            // Captura los datos siempre y cuando no pase los caracteres guardados y no sea el caracter un salto de linea.
            if (aux != '\n' && contador < Size - 1)
            {
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
                // Se encarga de mostrar por pantalla nos caracteres no validos.
                else
                {
                    validacion = false;
                }
            }
            // Se encarga de imprimir el número escrito.
            else if ((aux == '\n' || contador == Size) && validacion)
            {
                data = (float)dataReal + (float)dataDecimal / dataPow;
                Serial.println(data);
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
            else if (not(validacion) && (aux == '\n' || contador == Size))
            {
                dataReal = 0;
                dataDecimal = 0;
                dataPow = 1;
                isDecimalStage = false;
                contador = 0;
                validacion1 = true;
                validacion2 = true;
                validacion = true;
                return -1;
            }
        }
        return 0;
    }

    char *Asignacion_strings()
    {
        if (Variables::miBT.available())
        {
            Serial.println("Conectado");
            String datos = Variables::miBT.readString();
            if(datos.length() > 2)
            {
                char *variable = new char[datos.length() - 2];
                datos.toCharArray(variable, (datos.length() - 1));
                for (int unsigned i = 0; i < (datos.length() - 2); i++)
                {
                    Serial.print(variable[i]);
                }
                Serial.println(' ');
                delete[] variable;
            }
        }
        return nullptr;
    }
    void Captura_eventos()
    {
        char * list;
        while(list == nullptr)
        {
            list = Asignacion_strings();
        }
    }
}

/**
 * @note Implementación de las funciones de la manera más optima posible sin recurrir al uso de librerías.
 *
 * @attention Este es la implementación de funciones de testeo para comprobar el funcionamiento
 *            de la regresión de Matriz.
 */