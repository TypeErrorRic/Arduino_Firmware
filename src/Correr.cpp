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
        if(memoria.set_up(Variables_datos) == 0)
        {
            Serial.println('0');
        }
        run.init(array,Variables_datos.altura);
        var.conmutador = true;
        var.Valvula_manual = false;
        var.ejecucion = true;
        for (int i = 0; i < 5; i++) var.array[i] = false;
        Serial.println("Simulacion real del Diagrama de estados.");
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
    void calibracion_memoria(Memoria_no_volatil& memoria)
    {
        Variables_datos.t = long(10.5);
        Variables_datos.peso_total = 7.1;
        Variables_datos.radio_llave = 2.5;
        Variables_datos.suma_valores = 4.2;
        Variables_datos.Caudal_total = 5.2;
        memoria.Escritura_One(Variables_datos.t);
        memoria[1];
        memoria.Escritura_One(Variables_datos.peso_total);
        memoria[2];
        memoria.Escritura_One(Variables_datos.radio_llave);
        memoria[3];
        float aux3[3] = {10.2, 14.5, 19.6};
        memoria.Escritura_estructura(aux3);
        memoria[4];
        memoria.Escritura_One(Variables_datos.suma_valores);
        memoria[5];
        memoria.Escritura_One(Variables_datos.Caudal_total);
    }
};