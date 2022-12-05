#include <Arduino.h>
#include "../include/Sensores.h"

static Variables_Celda_Carga num;
static Variables_Correr2 var;
static short int estado20{};
static short int estado40{};
static short int estado60{};
static short int estado80{};
static short int estado90{};
static bool array[5]{};
static bool conmutador{true};
namespace Correr2
{
    void setup()
    {
        // put your setup code here, to run once:
        Serial.begin(9600);
        pinMode(13, OUTPUT);
        pinMode(static_cast<uint8_t>(sensores::Eletrovalvula), OUTPUT);
        pinMode(static_cast<uint8_t>(sensores::Alerta_visual), OUTPUT);
        pinMode(13, OUTPUT);
        for (int i = 0; i < 5; i++) array[i] = false;
        digitalWrite(13, HIGH);
        conmutador = true;
        var.Valvula_manual = false;
        var.ejecucion = true;
    }

    char Sensores_estado_6()
    {
        char aux3{'7'};
        bool finalizar{true};
        if (conmutador == true && not(var.Valvula_manual))
        {
            Serial.println("Para abrir valvula oprima a");
            do
            {
                while (Serial.available() > 0)
                {
                    aux3 = (char)Serial.read();
                    if(aux3 == 'a') var.Valvula_manual = true;
                    if (var.Valvula_manual)
                    {
                        Serial.println("Se ha abierto la valvula manual");
                        digitalWrite(12, HIGH);
                        digitalWrite(13, LOW);
                        Serial.readString();
                        while (var.Valvula_manual)
                        {
                            while (Serial.available() > 0)
                            {
                                char aux = (char)Serial.read();
                                estado90 = '5' == aux ? 1 : 0;
                                estado80 = '4' == aux ? 1 : 0;
                                estado60 = '3' == aux ? 1 : 0;
                                estado40 = '2' == aux ? 1 : 0;
                                estado20 = '1' == aux ? 1 : 0;
                                if (estado20 == 1)
                                {
                                    if (array[4] == false && array[3] == false && array[1] == false && array[2] == false)
                                    {
                                        Serial.println("Nivel del 20%: Alcanzado.");
                                        array[0] = false;
                                        var.Valvula_manual = false;
                                        Serial.println("Cerrar Valvula manual.");
                                        Serial.readString();
                                        bool pasar{true};
                                        Serial.println("Presione l para cerrar la valvula manual.");
                                        while(pasar)
                                        {
                                            while (Serial.available() > 0)
                                            {
                                                char abrir = (char)Serial.read();
                                                if (abrir == 'l')
                                                {
                                                    digitalWrite(12, LOW);
                                                    pasar = false;
                                                    Serial.println("Se ha cerrado Valvula manual");
                                                }
                                            }
                                        }
                                        digitalWrite(13, HIGH);
                                        finalizar = false;
                                        break;
                                    }
                                    else
                                    {
                                        var.ejecucion = false;
                                        Serial.println("Error");
                                        Serial.println("Vaciar el tanque primero.");
                                        finalizar = false;
                                    }
                                    break;
                                }
                                if (estado40 == 1)
                                {
                                    if (array[4] == false && array[0] == true && array[2] == false && array[3] == false)
                                    {
                                        Serial.println("Nivel del 40%: Alcanzado.");
                                        array[1] = false;
                                    }
                                    else
                                    {
                                        var.ejecucion = false;
                                        Serial.println("Error");
                                        Serial.println("Vaciar el tanque primero.");
                                        finalizar = false;
                                    }
                                    break;
                                }
                                if (estado60 == 1)
                                {
                                    if (array[4] == false && array[0] == true && array[1] == true && array[3] == false)
                                    {
                                        Serial.println("Nivel del 60%: Alcanzado.");
                                        array[2] = false;
                                    }
                                    else
                                    {
                                        var.ejecucion = false;
                                        Serial.println("Error");
                                        Serial.println("Vaciar el tanque primero.");
                                        finalizar = false;
                                    }
                                    break;
                                }
                                if (estado80 == 1)
                                {
                                    if (array[4] == false && array[0] == true && array[1] == true && array[2] == true)
                                    {
                                        Serial.println("Nivel del 80%: Alcanzado.");
                                        array[3] = false;
                                    }
                                    else
                                    {
                                        var.ejecucion = false;
                                        Serial.println("Error");
                                        Serial.println("Vaciar el tanque primero.");
                                        finalizar = false;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            } while ('5' != aux3 and finalizar);
        }
        else if (conmutador == false)
        {
            Serial.println("Se ha detenido el proceso");
            Serial.println("Vaciando Tanque");
            digitalWrite(13, LOW);
            digitalWrite(12, HIGH);
            delay(10000);
            aux3 = {'\0'};
            Serial.println("Tanque vaceado");
            digitalWrite(12, LOW);
            Serial.println("Fin de la jornada. Que tenga buen día");
        }
        return aux3;
    }
    void loop()
    {
        var.Valvula_manual = false;
        Serial.println("Se prendio la maquina");
        while (conmutador)
        {
            if (array[3] == true or array[0] == true or array[1] == true or array[2] == true or array[4] == true)
            {
                var.ejecucion = false;
                digitalWrite(13, LOW);
                char aux41{};
                while(aux41 != 'a')
                {
                    while (Serial.available() > 0)
                    {
                        char aux41 = Serial.read();
                        if (aux41 == 'a')
                        {
                            var.Valvula_manual = false;
                            var.ejecucion = true;
                            Serial.println("Se está vaceando el tanque.");
                            digitalWrite(12, HIGH);
                            delay(1000);
                            Serial.println("Cerrando valvula");
                            digitalWrite(12, LOW);
                            Serial.println("Listo para ser llenado de nuevo");
                        }
                    }
                }
                aux41 = '0';
            }
            bool exceso{false};
            bool pass{false};
            if (var.ejecucion)
            {
                digitalWrite(13, HIGH);
                char aux = {'\0'};
                Serial.println("Para terminar turno presionar t");
                Serial.println("Ingrese un valor: ");
                while (var.ejecucion)
                {
                    while (Serial.available() > 0)
                    {
                        aux = (char)Serial.read();
                        estado90 = '5' == aux ? 1 : 0;
                        if (estado90 == 1 || exceso)
                        {
                            if (array[3] == true && array[0] == true && array[1] == true && array[2] == true)
                            {
                                Serial.println("Nivel del 90%: Alcanzado.");
                                if (conmutador == true)
                                {
                                    array[4] = true;
                                    Serial.println("Abrir la valvula manual. Nivel del 90% alcanzado");
                                    digitalWrite(static_cast<uint8_t>(sensores::Alerta_visual), HIGH);
                                    Serial.println("Abrir Enserió.");
                                    do
                                    {
                                        while (Serial.available() > 0)
                                        {
                                            char abrir = (char)Serial.read();
                                            if (abrir == 'a')
                                            {
                                                digitalWrite(12, HIGH);
                                                digitalWrite(13, LOW);
                                                Serial.println("Se ha avierto");
                                                var.Valvula_manual = true;
                                            }
                                        }
                                        if (var.Valvula_manual)
                                            while (array[4] == true) while (Serial.available() > 0)
                                                {
                                                    char aux = Serial.read();
                                                    if (aux == '4')
                                                    {
                                                        array[4] = false;
                                                        pass = true;
                                                    }
                                                }
                                    } while (not(var.Valvula_manual));
                                }
                            }
                            else
                            {
                                var.ejecucion = false;
                                Serial.println("Error.");
                                Serial.println("Vaciar el tanque primero.");
                            }
                        }
                        estado80 = '4' == aux ? 1 : 0;
                        if (estado80 == 1 || pass)
                        {
                            if (array[4] == false && array[0] == true && array[1] == true && array[2] == true)
                            {
                                array[3] = true;
                                Serial.println("Nivel del 80%: superado.");
                                while ('4' == aux)
                                {
                                    aux = Sensores_estado_6();
                                    if(aux == '5') 
                                    {
                                        exceso = true;
                                    }
                                }
                            }
                            else
                            {
                                var.ejecucion = false;
                                Serial.println("Error.");
                                Serial.println("Vaciar el tanque primero.");
                            }
                        }

                        estado60 = '3' == aux ? 1 : 0;
                        if (estado60 == 1)
                        {
                            if (array[4] == false && array[0] == true && array[1] == true && array[3] == false)
                            {
                                Serial.println("Nivel del 60%: superado.");
                                array[2] = true;
                            }
                            else
                            {
                                var.ejecucion = false;
                                Serial.println("Error.");
                                Serial.println("Vaciar el tanque primero.");
                            }
                            break;
                        }

                        estado40 = '2' == aux ? 1 : 0;
                        if (estado40 == 1 and array[0] == true)
                        {
                            if (array[4] == false && array[0] == true && array[2] == false && array[3] == false)
                            {
                                Serial.println("Nivel del 40%: superado.");
                                array[1] = true;
                            }
                            else
                            {
                                var.ejecucion = false;
                                Serial.println("Error.");
                                Serial.println("Vaciar el tanque primero.");
                            }
                            break;
                        }

                        estado20 = '1' == aux ? 1 : 0;
                        if (estado20 == 1)
                        {
                            if (array[4] == false && array[1] == false && array[2] == false && array[3] == false)
                            {
                                Serial.println("Nivel del 20%: superado.");
                                array[0] = true;
                            }
                            else
                            {
                                var.ejecucion = false;
                                Serial.println("Erro.");
                                Serial.println("Vaciar el tanque primero.");
                            }
                            break;
                        }
                        if(aux == 't') 
                        {
                            conmutador = false;
                            Serial.println("Se termino el turno.");
                            break;
                        }
                    }
                }
            }
        }
    }
};
