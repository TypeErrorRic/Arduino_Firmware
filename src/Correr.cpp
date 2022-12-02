#include "../include/Correr.h"

// Estucutra que almacena los valores de los sensores.
static Variables::Valores_Sensores Medidas{};
static Variables_Celda_Carga Celda_Carga;
static HX711 balanza;
static Variables_Correr run;
static Caudal Operaciones{run.densidad,run.radio};

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
        balanza.tare(20);             // El peso actual es considerado Tara.

        Serial.println("Listo para pesar");
        pinMode(static_cast<uint8_t>(sensores::Eletrovalvula), OUTPUT);
        for(short i=2; i < run.Num_elementos;i++) pinMode(i, INPUT);
    }
    void loop()
    {
        Serial.print("Peso: ");
        Serial.print(balanza.get_units(20), 3);
        Serial.println(" kg");
        delay(500);
        run.peso = balanza.get_units(20);
        if(0 > run.peso > 0.5)
        {
            Serial.println("Abriendo eletrovalvula");
            while(run.ejecucion)
            {
                digitalWrite(static_cast<uint8_t>(sensores::Eletrovalvula), HIGH);
                switch (Sensor)
                {
                case sensores::Sensor1:
                    Serial.println("Se a activado el sensor # 1.");
                    break;
                case sensores::Sensor2:
                    Serial.println("Se a activado el sensor # 2.");
                case sensores::Sensor3:
                    Serial.println("Se a activado el sensor # 3.");
                    break;
                case sensores::Sensor4:
                    Serial.println("Se a activado el sensor # 4.");
                    if (digitalRead(static_cast<uint8_t>(sensores::comutador)) == 1 && run.Valvula_manual)
                    {
                        while (digitalRead(static_cast<uint8_t>(sensores::Sensor4) == 1))
                        {
                            if (digitalRead(static_cast<uint8_t>(sensores::Sensor5) == 1))
                            {
                                Serial.println("Se sigue llenando el tanque");
                                break;
                            }
                        }
                    }
                    else if (digitalRead(static_cast<uint8_t>(sensores::comutador)) == 0)
                    {
                       
                    }
                    break;
                case sensores::Sensor5:
                    Serial.println("Se a activado el sensor # 5.");
                    if (digitalRead(static_cast<uint8_t>(sensores::comutador)) == 1)
                    {
                        Serial.println("Abrir la valvula manual. Nivel del 90% alcanzado");
                    }
                    break;
                default:
                    break;
                }
            }
        }
        else Serial.println("Se debe de abrir la Eletrovalvula");
    }
}; // namespace Correr
