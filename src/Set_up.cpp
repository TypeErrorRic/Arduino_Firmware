#include "..\include\Set_up.h"

// Estucutra que almacena los valores de los sensores.
static Variables::Valores_Sensores Medidas{};
// Insanciación de un obecto de la clase::<Regresion_Cuadratica.h>
static Regresion_Cuadratica Matriz(Variables::Nfilas, Medidas.x, Medidas.y);
//Miembros Y arreglos para el uso y configuración de los modulos:
static Variables_Celda_Carga Celda_Carga;
static HX711 balanza;
static Variables_Calibracion Calibrar;

namespace Calibracion
{
    void setup()
    {
        Serial.begin(9600);
        balanza.begin(Celda_Carga.HX711_dout, Celda_Carga.HX711_sck);
        Serial.print("Lectura del valor del ADC:t");
        Serial.println(balanza.read());
        Serial.println("No ponga ningún objeto sobre la balanza");
        Serial.println("Destarando...");
        balanza.set_scale(); // La escala por defecto es 1
        balanza.tare(20);    // El peso actual es considerado Tara.
        for (int i = 0; i < Variables::Nfilas; i++)
        {
            Medidas.x[i] = 0;
            Medidas.y[i] = 0;
        }
        Matriz.reset();
        pinMode(LED_BUILTIN, OUTPUT); // Configuración del led.
        Serial.println("Setup conluido correctamente.");
        Serial.println("Menu de opciones:");
        Serial.println("1. Oprimir 'c' para configurar la celda de carga.");
        Serial.println("2. Oprimir 'm' para configurar los niveles de altura.");
    }

    void loop()
    {
        // receive command from serial terminal
        if (Serial.available() > 0)
        {
            char inByte = Serial.read();
            if (inByte == 'c')
            {
                Serial.print("Coloque un peso conocido. ");
                Serial.println("Peso: ");
                while(Calibrar.peso == 0) Calibrar.peso = Testeo::Lectura_events();
                for (short int i = 0; i <= Calibrar.Calculo_Peso; i++)
                {
                    Serial.print("Valor de lectura: t");
                    Serial.println(balanza.get_value(10), 0);
                    Calibrar.suma_valores += balanza.get_value(10)/Calibrar.peso;
                    delay(100);
                }
                EEPROM.put(Celda_Carga.calVal_eepromAdress, (Calibrar.suma_valores / Calibrar.Calculo_Peso));
                Serial.print("Peso: ");
                Serial.println((Calibrar.suma_valores / Calibrar.Calculo_Peso),0);
                Serial.println("Dato configurado corretamente");
            }
            else if (inByte == 'm')
            {
                int contador_num{0};
                digitalWrite(LED_BUILTIN, LOW);
                Serial.println("Calculando....");
                // Bucle principal de toma de datos:
                do
                {
                    delay(1000);
                    Testeo::Serial_events(Medidas.x[contador_num], Medidas.y[contador_num], Matriz.get_Realizar());
                    contador_num++;
                    // Si se ha realizado la toma de valores con un patron incapaz de ser acomodados en una curva cuadratica. Lazara error.
                    if (Matriz.get_Realizar())
                    {
                        Serial.print("Coordenada #: ");
                        Serial.print(contador_num);
                        Serial.println(" Tomada correctamente.");
                    }
                } while (Matriz.Update(Medidas.x[contador_num - 1], Medidas.y[contador_num - 1]));
                digitalWrite(LED_BUILTIN, LOW);
                Serial.println("..................");
                Serial.println("Datos Ingresados:");
                delay(1000);
                digitalWrite(LED_BUILTIN, HIGH);
                // Si se ha realizado la toma de valores con un patron incapaz de ser acomodados en una curva cuadratica. Lazara error.
                if (Matriz.get_Realizar())
                {
                    Matriz.get_Matriz();
                    Matriz.Calcular();
                    Serial.println("Configuración realizada correctamente");
                }
                else
                    Serial.print("Reiniciar.");
                delay(1000);
            }
        }
    }
}; // namespace Set_ip
