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
        array[2] = Variables_datos.values.x0;
        run.init(array,Variables_datos.altura);
        var.conmutador = true;
        var.Valvula_manual = false;
        var.ejecucion = true;
        for (int i = 0; i < 5; i++) var.array[i] = false;
        //Serial.println("Simulacion real del Diagrama de estados.");
    }
    void loop()
    {
        if (var.conmutador)
        {
            run.Encender(var.array);
            run.llenado_nivel(var.array, false);
        }
        else if(Serial.available() > 0)
        {
            auto comuntar_master = (char)Serial.read();
            if (comuntar_master == 't')
            {
                var.conmutador = true;
                Serial.println("Conmutador: ON");
                Serial.readString();
            }
        }
    }
    void calibracion(bool &validacion)
    {
        while(validacion)
        {
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
                    while(true)
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
                }
            }
            //Limpiar:
            else if(list[0] == "4")
            {
                Manejo_datos::limpiar_data();
                if(Manejo_datos::inicializar())
                    Variables_datos = Manejo_datos::get_datos();
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
            delete[] list;
        }
    }
}