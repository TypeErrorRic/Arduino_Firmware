#include "../include/Manejo_datos.h"

static Variables_Guardar Variables_datos;
static Memoria_no_volatil memoria;

namespace Manejo_datos
{
    void inicializar_default()
    {
        memoria.limpiar();
        memoria.Escritura_One(Variables_datos.tiempo);
        memoria[1];
        memoria.Escritura_One(Variables_datos.peso_total);
        memoria[2];
        memoria.Escritura_One(Variables_datos.radio_llave);
        memoria[3];
        memoria.Escritura_One(Variables_datos.values);
        memoria[4];
        memoria.Escritura_One(Variables_datos.suma_valores);
        memoria[5];
        memoria.Escritura_One(Variables_datos.Caudal_total);
        memoria[6];
        memoria.Escritura_One(Variables_datos.altura);
        Variables_datos.t = 20;
        memoria[7];
        memoria.Escritura_One(Variables_datos.t);
        memoria[8];
        memoria.inicializar_list();
    }

    void set_up()
    {
        memoria[0];
        Variables_datos.tiempo = memoria.inicializar();
        memoria[1];
        if (memoria.Lectura(Variables_datos.peso_total))
        {
            memoria[2];
            if (memoria.Lectura(Variables_datos.radio_llave))
            {
                memoria[3];
                if (memoria.Lectura(Variables_datos.values))
                {
                    memoria[4];
                    if (memoria.Lectura(Variables_datos.suma_valores))
                    {
                        memoria[5];
                        if (memoria.Lectura(Variables_datos.Caudal_total))
                        {
                            memoria[6];
                            if (memoria.Lectura(Variables_datos.altura))
                            {
                                memoria[8];
                                if (memoria.Lectura_lista(Variables_datos.lista_dias))
                                {
                                    memoria[7];
                                    if (memoria.Lectura(Variables_datos.t))
                                    {
                                        Serial.println("0");
                                    }
                                    else
                                        Serial.println("6");
                                }
                            }
                        }
                        else
                            Serial.println("5");
                    }
                }
                else
                    Serial.println("4");
            }
            else
                Serial.println("3");
        }
        else
            Serial.println("2");
    }

    void Guardar_regresion(float array[3])
    {
        Variables_datos.values.x0 = array[0];
        Variables_datos.values.x1 = array[1];
        Variables_datos.values.x0 = array[2];
        memoria[3];
        memoria.Escritura_One(Variables_datos.values);
    }

    void Guardar_config_celda_carga(long &tara, float &suma)
    {
        Variables_datos.t = tara;
        memoria[7];
        memoria.Escritura_One(Variables_datos.t);
        Variables_datos.suma_valores = suma;
        memoria[4];
        memoria.Escritura_One(Variables_datos.suma_valores);
    }

    void guarda_config_generales(float &peso, float &altura)
    {
        Variables_datos.peso_total = peso;
        memoria[1];
        memoria.Escritura_One(Variables_datos.peso_total);
        Variables_datos.altura = altura;
        memoria[6];
        memoria.Escritura_One(Variables_datos.altura);
    }

    void guardar_peso_diario(float &dias)
    {
        memoria[8];
        memoria.Escritura_lista(dias, 30);
    }

    void limpiar_data()
    {
        memoria.limpiar();
        inicializar_default();
    }

    bool inicializar()
    {
        if (memoria.get_id() != 1)
        {
            set_up();
            return true;
        }
        return false;
    }

    Variables_Guardar &get_datos()
    {
        return Variables_datos;
    }
};