#include "../include/Correr.h"

// Estucutra que almacena los valores de los sensores.
static Variables::Valores_Sensores Medidas{};
static Variables_Celda_Carga Celda_Carga;
static HX711 balanza;
static Variables_Correr run;
static Caudal Operaciones{run.densidad, 7.1f, run.radio};

typedef enum sensores sensor;

sensor Sensor; 

namespace Correr
{
    void setup()
    {
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
        balanza.set_scale(EEPROM.get(Celda_Carga.calVal_eepromAdress, run.data)); // Establecemos la escala
        balanza.tare(20); // El peso actual es considerado Tara.

        Serial.println("Listo para pesar");
        pinMode(static_cast<uint8_t>(sensores::Eletrovalvula), OUTPUT);
        pinMode(static_cast<uint8_t>(sensores::Alerta_visual), OUTPUT);
        for(short i=2; i < run.Num_elementos;i++) pinMode(i, INPUT);
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
