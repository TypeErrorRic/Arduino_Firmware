#include "../include/Correr.h"
#include <Functional.h>

static Variables_Correr var;
static Functional run(var.ejecucion,var.Valvula_manual,var.conmutador);

namespace Correr
{
    void setup()
    {
        Serial.begin(Variables::VELOCIDAD_DATOS);
        run.init();
        var.conmutador = true;
        var.Valvula_manual = false;
        var.ejecucion = true;
        for (int i = 0; i < 5; i++) var.array[i] = false;
        Serial.println("Simulacion real del Diagrama de estados.");
        Serial.println("Se le deben ingresar valores numericos que representan la activacion.");
        Serial.println("El numero 1 representa: Nivel 20%");
        Serial.println("El numero 2 representa: Nivel 40%");
        Serial.println("El numero 3 representa: Nivel 60%");
        Serial.println("El numero 4 representa: Nivel 80%");
        Serial.println("El numero 1 representa: Nivel 90%");
        Serial.println("Para abrir/cerrar la Valvula Manual presionar a.");
        Serial.println("Para apagar/prender conmutador presionar t.");
        Serial.println('\n');
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
};