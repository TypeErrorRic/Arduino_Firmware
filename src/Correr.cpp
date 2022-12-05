#include "../include/Correr.h"
#include "..\include\Testeo.h"

// Estucutra que almacena los valores de los sensores.
static Variables_Celda_Carga Celda_Carga;
static HX711 balanza;
static Variables_Correr run;
static Variables_Celda_Carga num;
static Caudal Operaciones{Celda_Carga.densidad, Celda_Carga.Valores.peso_total, Celda_Carga.Valores.radio_llave};

//Valores para los sensores
static sensor Sensor; 

namespace Correr
{
    void setup()
    {
        Memoria_light men;
        men.Escritura(Celda_Carga.Valores);
        Serial.begin(Variables::VELOCIDAD_DATOS);
        delay(10);
        Serial.println();
        Serial.println("Starting...");
        balanza.begin(Celda_Carga.HX711_dout, Celda_Carga.HX711_sck);
        Serial.print("Lectura del valor del ADC:  ");
        Serial.println(balanza.read());
        Serial.println("No ponga ningun  objeto sobre la balanza");
        Serial.println("Destarando...");
        Serial.println("...");
        balanza.set_scale(Celda_Carga.Valores.suma_valores); // Establecemos la escala
        balanza.tare(20); // El peso actual es considerado Tara.

        Serial.println("Listo para pesar");
        pinMode(static_cast<uint8_t>(sensores::Eletrovalvula), OUTPUT);
        pinMode(static_cast<uint8_t>(sensores::Alerta_visual), OUTPUT);
        for(short i=2; i <= num.Num_elementos;i++) pinMode(i, INPUT);
        EEPROM.get(0, Celda_Carga.Valores);
    }

    void Sensores_estado_6()
    {
        if (digitalRead(static_cast<uint8_t>(sensores::comutador)) == 1 && not(run.Valvula_manual))
        {
            digitalWrite(static_cast<uint8_t>(sensores::Alerta_visual), HIGH);
            do
            {
                run.Valvula_manual = Operaciones.estado(run.peso) == 1 ? false : true;
                if (run.Valvula_manual)
                {
                    Serial.println("Se ha abierto la valvula manual");
                    break;
                }
            } while (digitalRead(static_cast<uint8_t>(sensores::Sensor4) == 1));
        }
        else if (digitalRead(static_cast<uint8_t>(sensores::comutador)) == 0)
        {
            Serial.println("Se ha detenido el proceso");
        }
        digitalWrite(static_cast<uint8_t>(sensores::Eletrovalvula), HIGH);
    }

    void loop()
    {
        Serial.print("Peso: ");
        Serial.print(balanza.get_units(20), 3);
        Serial.println(" kg");
        delay(500);
        run.peso = balanza.get_units(20);
        if((0 <= run.peso && run.peso <= 0.5) && run.ejecucion == true)
        {
            Serial.println("Abriendo eletrovalvula");
            digitalWrite(static_cast<uint8_t>(sensores::Eletrovalvula), HIGH);
            while (run.ejecucion && (0 <= run.peso && run.peso <= 0.5))
            {
                switch (Sensor)
                {
                case sensores::Sensor1:
                    Serial.println("Se a activado el sensor # 1.");
                    while (Sensor == sensor::Sensor1)
                    {
                        // Verificar sensor.
                    }
                    break;
                case sensores::Sensor2:
                    Serial.println("Se a activado el sensor # 2.");
                    while (Sensor == sensor::Sensor2)
                    {
                        // Verificar sensor.
                    }
                case sensores::Sensor3:
                    Serial.println("Se a activado el sensor # 3.");
                    while(Sensor == sensor::Sensor3)
                    {
                        //Verificar sensor.
                    }
                    break;
                case sensores::Sensor4:
                    Serial.println("Se a activado el sensor # 4.");
                    Serial.println("Abrir electrovalvula");
                    while (Sensor == sensor::Sensor4)
                    {
                        Sensores_estado_6();
                    }
                    break;
                case sensores::Sensor5:
                    Serial.println("Se a activado el sensor # 5.");
                    if (digitalRead(static_cast<uint8_t>(sensores::comutador)) == 1)
                    {
                        Serial.println("Abrir la valvula manual. Nivel del 90% alcanzado");
                        digitalWrite(static_cast<uint8_t>(sensores::Alerta_visual), HIGH);
                        do
                        {
                            Serial.println("Abrir Enserió.");
                            if(run.Valvula_manual) while (Sensor == sensor::Sensor5);
                        } 
                        while (not(run.Valvula_manual));
                    }
                    break;
                default:
                    Operaciones.Valores(run.peso);
                    break;
                delay(1000);
                }
            }
        }
        else Serial.println("Se debe de abrir la Valvula manual");
    }
}; // namespace Correr
