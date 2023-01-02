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
        Serial.begin(Variables::VELOCIDAD_DATOS);
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
    void calibracion(bool &validdacion)
    {
        while(validdacion)
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
            }
            //Altura y Peso total:
            else if (list[0] == "1" && size == 3)
            {
                Variables_datos.altura = Lectura_data::Convertir_a_Numero(list[0]);
                Variables_datos.peso_total = Lectura_data::Convertir_a_Numero(list[1]);
                Manejo_datos::guarda_config_generales(Variables_datos.peso_total, Variables_datos.altura);
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
                            if(listas[1] == '1')
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
            else if(list[0] == "4")
            {
                Manejo_datos::limpiar_data();
                if(Manejo_datos::inicializar())
                    Variables_datos = Manejo_datos::get_datos();
            }
            else if(list[0] == "5")
            {
                delete[] list;
                break;
            }
            delete[] list;
        }
        validdacion = false;
    }
}