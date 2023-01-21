#include "../include/Correr.h"
#include <Functional.h>

//Miembros estaticos incializados
static Variables_Correr var;
static Variables_config config;
static Variables_Guardar Variables_datos;
static float array[3]{};

//Instanciación de clases.
static Caudal caudal(config.densidad, Variables_datos.peso_total, Variables_datos, Variables_datos.Caudal_total);
static Functional run(caudal,var.ejecucion,var.Valvula_manual,var.conmutador);

namespace Correr
{
    void setup()
    {
        Memoria_no_volatil memoria;
        if(Manejo_datos::inicializar())
            Variables_datos = Manejo_datos::get_datos();
        array[0] =Variables_datos.values.x0;
        array[1] = Variables_datos.values.x1;
        array[2] = Variables_datos.values.x2;
        run.init(array,Variables_datos.altura);
        var.conmutador = false;
        var.Valvula_manual = false;
        var.ejecucion = true;
        for (int i = 0; i < 5; i++) var.array[i] = false;
    }
    void loop()
    {
        Variables::Lcd.setCursor(0, 0);
        if(digitalRead(10) == 0)
        Variables::Lcd.print("Estado S0");
        if (var.conmutador)
        {
            Variables::Lcd.clear();
            Variables::Lcd.print("Estado S1.");
            delay(1000);
            run.Encender(var.array);
            run.llenado_nivel(var.array, false);
        }
        else if(digitalRead(10) == 1)
        {
            var.conmutador = true;
            Variables::Lcd.setCursor(0,0);
            Variables::Lcd.print("Conmutador ON");
            delay(1000);
        }
    }

    void calibracion(bool &validacion)
    {
        while(validacion)
        {
            Variables::Lcd.setCursor(0, 0);
            Variables::Lcd.print("Calibrando...");
            short size{0};
            String *list = Lectura_data::Captura_eventos(size);
            //Regresion Cuadratica:
            if (list[0] == "0" && size == 4)
            {
                array[0] = Lectura_data::Convertir_a_Numero(list[1]);
                array[1] = Lectura_data::Convertir_a_Numero(list[2]);
                array[2] = Lectura_data::Convertir_a_Numero(list[3]);
                Manejo_datos::Guardar_regresion(array);
                for(short i=0; i<3; i++)
                    Serial.println(array[i]);
            }
            //Peso total:
            else if (list[0] == "1" && size == 2)
            {
                Variables_datos.peso_total = Lectura_data::Convertir_a_Numero(list[1]);
                Manejo_datos::guarda_config_peso(Variables_datos.peso_total);
                Serial.println(Variables_datos.peso_total);
            }
            //Celdad de carga:
            else if(list[0] == "2" && size == 2)
            {
                bool realizar{false};
                char *listas = nullptr;
                short size{0};
                if(list[1] == "true")
                {
                    Variables::Lcd.clear();
                    Variables_datos.t = 0;
                    while (true)
                    {
                        if(caudal.calibracion_escala(Variables_datos.t, Variables_datos.suma_valores, realizar))
                            break;
                        listas = Lectura_data::Asignacion_strings(size);
                        if (listas != nullptr)
                        {
                            if(listas[1] == 't')
                            {
                                realizar = true;
                            }
                            delete[] listas;
                        }
                    }
                    Manejo_datos::Guardar_config_celda_carga(Variables_datos.t, Variables_datos.suma_valores);
                }
            }
            //Días:
            else if (list[0] == "3" && size == 2)
            {
                if(list[1] == "true")
                {
                    for(short i = 0; i < 30; i++)
                    {
                        Serial.println(Variables_datos.lista_dias[i]);
                    }
                    Serial.println(Variables_datos.tiempo.dia);
                }
            }
            //Limpiar valores de la lista:
            else if(list[0] == "4")
            {
                //Manejo_datos::limpiar_data(); //Reinicia todos los datos. No activar aquí
                if(list[1] == "true")
                    if(Manejo_datos::inicializar())
                    {
                        Variables_datos = Manejo_datos::borrar_list();
                        for (short i = 0; i < 30; i++)
                        {
                            Serial.println(Variables_datos.lista_dias[i]);
                        }
                    }
            }
            //Salir y confirmar:
            else if(list[0] == "5")
            {
                if(list[1] == "true")
                    Serial.println("sigue");
                else if (list[1] == "false")
                {
                    delete[] list;
                    validacion = false;
                    break;
                }
            }
            //Conseuir datos almacenados:
            else if(list[0] == "6")
            {
                if(list[1] == "true")
                {
                    Serial.println(Variables_datos.t);
                    Serial.println(Variables_datos.peso_total);
                    Serial.println(Variables_datos.radio_llave);
                    Serial.println(Variables_datos.suma_valores);
                    Serial.println(Variables_datos.Caudal_total);
                    Serial.println(Variables_datos.altura);
                }
                else
                    Serial.println("00");
            }
            //Altura_maxima
            else if(list[0] == "7")
            {
                Variables_datos.altura = Lectura_data::Convertir_a_Numero(list[1]);
                Manejo_datos::guarda_config_peso(Variables_datos.altura);
                Serial.println(Variables_datos.altura);
            }
            //Valores x de la regresion:
            else if(list[0] == "8")
            {
                if (list[1] == "true")
                {
                    float array_regr[5];
                    caudal.Calibracion_niveles(array_regr);
                    Serial.println(array_regr[0]);
                    Serial.println(array_regr[1]);
                    Serial.println(array_regr[2]);
                    Serial.println(array_regr[3]);
                    Serial.println(array_regr[4]);
                }
            }
            delete[] list;
        }
    }
}