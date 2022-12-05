#include "..\include\Set_up.h"
#include "..\include\Testeo.h"

// Estucutra que almacena los valores de los sensores.;
static Variables_Celda_Carga Celda_Carga;
// Insanciación de un obecto de la clase::<Regresion_Cuadratica.h>
static Regresion_Cuadratica Matriz(Nfilas, Celda_Carga.x, Celda_Carga.Valores.y);
// Miembros Y arreglos para el uso y configuración de los modulos:
static HX711 balanza;
static Variables_Calibracion Calibrar;

static sensor Sensor;

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

        for (short i = 2; i <= Celda_Carga.Num_elementos; i++)
            pinMode(i, INPUT);

        for (int i = 0; i < Nfilas; i++)
        {
            Celda_Carga.x[i] = 0;
            Celda_Carga.Valores.y[i] = 0;
        }
        Matriz.reset();
        pinMode(LED_BUILTIN, OUTPUT); // Configuración del led.
        Serial.println("Setup conluido correctamente.");
        Serial.println("Menu de opciones:");
        Serial.println("1. Oprimir 'c' para configurar la celda de carga.");
        Serial.println("2. Oprimir 'm' para configurar los niveles de altura.");
        Serial.println("3. Oprimir 'g' para guardar.");
        Serial.println("4. Oprimir 'w' para configurar los parametros de ejecución.");
        Serial.println("No se olvide de guardar los datos tomados.");
        EEPROM.get(0,Celda_Carga.Valores);
    }

    void loop()
    {
        char inByte{'\0'};
        // receive command from serial terminal
        do
        {
            while (Serial.available() > 0)
            {
                auto inbyte = (char)Serial.read();
                if (inbyte != '\n' && inbyte != '\0') 
                {
                    inByte = inbyte;
                }
            }
        } 
        while (inByte == '\0');
        if (inByte == 'c')
        {
            short aux = 0;
            char aux2 = {'\0'};
            Serial.print("Coloque un peso conocido. ");
            Serial.println("Peso: ");
            Serial.println("Para salir presione la tecla e");
            while (not(aux2 == 'e'))
            {
                while (Serial.available() > 0)
                {
                    aux2 = Serial.read();
                    if(aux2 == 'e') break;
                }
                Calibrar.peso = Testeo::Lectura_events();
                Serial.print("Valor de lectura: t");
                Serial.println(balanza.get_value(10), 0);
                Calibrar.array[aux%max_value] = (balanza.get_value(10) / Calibrar.peso);
                delay(100);
                aux++;
            }
            for(short i=0; i<max_value; i++) Celda_Carga.Valores.suma_valores += (Calibrar.array[i]/Celda_Carga.Valores.peso_total);
            Serial.print("Peso: ");
            Celda_Carga.Valores.suma_valores = (Celda_Carga.Valores.suma_valores / Calibrar.Calculo_Peso);
            Serial.println((Celda_Carga.Valores.suma_valores / Calibrar.Calculo_Peso), 0);
            Serial.println("Dato configurado corretamente");
            Serial.readString();
        }
        else if (inByte == 'w')
        {
            Testeo::Valores(Celda_Carga.Valores.peso_total, Celda_Carga.Valores.radio_llave);
        }
        else if (inByte == 'm' && (Celda_Carga.Valores.peso_total != 0 && Celda_Carga.Valores.radio_llave != 0) /*&& Celda_Carga.Valores.suma_valores != 0*/)
        {
            int contador_num{0};
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("Calculando....");
            // Bucle principal de toma de datos:
            float aux{};
            Serial.println("Asegurese de que el tanque se este llenando.");
            delay(5000);
            do
            {
                delay(1000);
                aux = contador_num > 0 ? Celda_Carga.Valores.y[contador_num - 1] : 0;
                Testeo::Serial_events(aux, Celda_Carga.Valores.y[contador_num], Matriz.get_Realizar());
                contador_num++;
                // Si se ha realizado la toma de valores con un patron incapaz de ser acomodados en una curva cuadratica. Lazara error.
                if (Matriz.get_Realizar())
                {
                    Serial.print("Coordenada #: ");
                    Serial.print(contador_num);
                    Serial.println(" Tomada correctamente.");
                }
            } while (Matriz.Update(Celda_Carga.Valores.y[contador_num - 1]));
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("..................");
            Serial.println("Datos Ingresados:");
            Calibrar.time = millis();
            balanza.set_scale(Celda_Carga.Valores.suma_valores);
            do
            {
                if (digitalRead(static_cast<uint8_t>(sensores::Sensor5)))
                {
                    Serial.println("Se ha estableciod el valor # 5");
                    Matriz.Update_x(balanza.get_units(20));
                    Calibrar.velocidad = ((Celda_Carga.Valores.peso_total * Celda_Carga.densidad) / 9.8) / Calibrar.time;
                    break;
                }
                else if (digitalRead(static_cast<uint8_t>(sensores::Sensor4)))
                {
                    Serial.println("Se ha estableciod el valor # 4");
                    Matriz.Update_x(balanza.get_units(20));
                }
                else if (digitalRead(static_cast<uint8_t>(sensores::Sensor3)))
                {
                    Serial.println("Se ha estableciod el valor # 3");
                    Matriz.Update_x(balanza.get_units(20));
                }
                else if (digitalRead(static_cast<uint8_t>(sensores::Sensor2)))
                {
                    Serial.println("Se ha estableciod el valor # 2");
                    Matriz.Update_x(balanza.get_units(20));
                }
                else if (digitalRead(static_cast<uint8_t>(sensores::Sensor1)))
                {
                    Serial.println("Se ha estableciod el valor # 1");
                    Matriz.Update_x(balanza.get_units(20));
                }
            } while (Sensor != sensor::Sensor5);
            delay(1000);
            digitalWrite(LED_BUILTIN, HIGH);
            // Si se ha realizado la toma de valores con un patron incapaz de ser acomodados en una curva cuadratica. Lazara error.
            if (Matriz.get_Realizar())
            {
                Matriz.get_Matriz();
                Matriz.Calcular();
                Serial.println("Configuración realizada correctamente");
                Celda_Carga.Valores.valores[0] = Matriz.Get_valor_x2();
                Celda_Carga.Valores.valores[1] = Matriz.Get_valor_x1();
                Celda_Carga.Valores.valores[2] = Matriz.Get_valor_a();
            }
            else
                Serial.print("Reiniciar.");
            delay(1000);
        }
        else if (inByte == 'g')
        {
            if (Celda_Carga.Valores.peso_total != 0 && Celda_Carga.Valores.suma_valores != 0 && Celda_Carga.Valores.valores[2] != 0)
            {
                EEPROM.put(0, Celda_Carga.Valores);
                Serial.println("Datos Guardados");
            }
            Serial.println("Error en grabar los datos, no han sido actualizados correctamente.");
        }
        else if(inByte != '\0')
        {
            Serial.print("Opción no valida. El caracter: ");
            Serial.print(inByte);
            Serial.println(". No es valido");
        }
        inByte = '\0';
    }
}; // namespace Set_ip
