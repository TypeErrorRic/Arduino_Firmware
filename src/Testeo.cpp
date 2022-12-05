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

#include "..\include\Testeo.h"

// Variables globales de las funciones:
static int dataReal{0}, dataDecimal{0}, dataPow{1};
static bool isDecimalStage{false}, validacion{true}, validacion1{true}, validacion2{true};
static float data{0};
static unsigned short int contador{0};
static bool siguiente{true};

/**
 * @brief Ambito de Teseto.
 */
namespace Testeo
{
    /**
     * @brief Se encarga de recivir los números enteros
     *
     * @return float
     */
    float Lectura_events()
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
                    Serial.print("No es un caracter valido: ");
                    Serial.println(aux);
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
                Serial.println("Error en la lectura de datos.");
                Serial.println("Ingrese otro Valor: ");
                data = -1;
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
        return data;
    }

    /**
     * @brief Se encarga de guardar los valores en los vectores correspondientes
     *
     * @param num1 Valor de la coordenada X.
     * @param num2 Valor de la coordenada Y.
     */
    void Serial_events(float &num1, float &num2, bool &realizar)
    {
        if (realizar)
        {
            // Se encara de pedir cada uno de los pares númericos a los cuales se les quiere conseguir la regresión cuadratica.
            Serial.read();
            Serial.println("Ingrese el valor de la altura en metros: ");
            bool confirmacion = true;
            siguiente = true;
            while (confirmacion)
            {
                while(siguiente) num2 = Lectura_events();
                if (num2 >= num1) confirmacion = false;
                else
                {
                    siguiente = true;
                    Serial.println("Valor incorrecto: Menor al ingresado anteriormente.");
                }
            }
            siguiente = true;
            data = 0;
        }
        else
            Serial.println("No valido.");
    }

    void Valores(float &num1, float &num2)
    {
        bool confirmacion{true};
        siguiente = true;
        Serial.readString();
        Serial.println("Ingrese el valor del Peso total del contendor: ");
        while (confirmacion) 
        {
            while(siguiente) 
            {
                num1 = Lectura_events();
            }
            if(num1 != 0) confirmacion = false;
            else 
            {
                Serial.println("0 no es un valor valido. Ingrese otro valor.");
                siguiente = true;
            }
        }
        siguiente = true;
        confirmacion = true;
        Serial.println("Ingrese el valor del radio: ");
        while (confirmacion)
        {
            while(siguiente) num2 = Lectura_events();
            if (num2 != 0) confirmacion = false;
            else
            {
                siguiente = true;
                Serial.println("0 no es un valor valido. Ingrese otro valor.");
            }
        }
        Serial.println("Datos guardados correctemente");
        siguiente = true;
    }
    /*
    int numeros()
    {
        int data{0};
        while(Serial.available() > 0)
        {
            char aux = (char)Serial.read();
            data = (aux >= '0' && aux <= '9') ? (dataDecimal * 10) + (aux - '0'): 0;
        }
        return data;
    }
    */
    void memorias(const Variables_Celda_Carga &guardar)
    {
        bool seguir{true};
        Memoria_light mem;
        Serial.println("Menu de la memoria:");
        Serial.println("*Para borrar la memoria oprima b.");
        // Serial.println("*Para borrar la memoria oprima r.");
        Serial.println("*Para ver los datos almacenados oprima m");
        Serial.println("*Para guardar los datos almacenados oprima g");
        while (seguir)
        {
            while (Serial.available() > 0)
            {
                char aux = (char)Serial.read();
                if (aux == 'm')
                {
                    mem.imprimir();
                    seguir = false;
                }
                else if (aux == 'r')
                {
                    /*
                    int aux = 0;
                    while(numeros() != 0) aux = numeros();
                    mem[aux];
                    mem.reemplazar_dato(Lectura_events());
                    seguir = false;
                    */
                }
                else if (aux == 'b')
                {
                    mem.limpiar();
                }
                else if(aux == 'g')
                {
                    mem.Escritura(guardar.Valores);
                }
                else
                    Serial.println("Dato no valido");
            }
        }
    }
}

/**
 * @note Implementación de las funciones de la manera más optima posible sin recurrir al uso de librerías.
 *
 * @attention Este es la implementación de funciones de testeo para comprobar el funcionamiento
 *            de la regresión de Matriz.
 */