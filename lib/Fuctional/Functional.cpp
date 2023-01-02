#include <Functional.h>

sensores Functional::sensor{};

void Functional::init(float array[3], float &altura)
{
    //Inicialicializando puertos:
    pinMode(sensor.ELECTROVALVULA, OUTPUT);
    pinMode(sensor.VALVULA_MANUAL, OUTPUT);
    pinMode(sensor.ALARMA, OUTPUT);
    pinMode(sensor.CONUMTADOR, OUTPUT);
    //Inicializando estado de los puertos:
    digitalWrite(sensor.ELECTROVALVULA, LOW);
    digitalWrite(sensor.VALVULA_MANUAL, LOW);
    digitalWrite(sensor.ALARMA, LOW);
    digitalWrite(sensor.CONUMTADOR, LOW);
    caudal.set_valores(array, altura);
}

void Functional::vericacion_vaciado(bool array[5])
{
    if (array[3] == true or array[0] == true or array[1] == true or array[2] == true or array[4] == true)
    {
        //Serial.println("Vaciar tanque Primero.");
        ejecucion = false;
        abrir_valvula();
        delay(5000);
        //Serial.println("Cerrar Valvula manual.");
        while (cerrar_valvula(array))
            ;
        for (int i = 0; i < 5; i++)
            array[i] = false;
        //Serial.println("Eletrovalvula: ON");
        ejecucion = true;
        digitalWrite(sensor.ELECTROVALVULA, HIGH);
    }
    else
    {
        //Serial.println("Eletrovalvula: ON");
        digitalWrite(sensor.ELECTROVALVULA, HIGH);
        ejecucion = true;
    }
}

bool Functional::cerrar_valvula(bool array[5])
{
    while (Serial.available() > 0)
    {
        aux = (char)Serial.read();
        if (aux == 'l')
        {
            //Serial.println("Valvula Manual: OFF");
            digitalWrite(sensor.VALVULA_MANUAL, LOW);
            aux = '\0';
            return false;
        }
    }
    aux = '\0';
    return true;
}

char &Functional::abrir_valvula()
{
    digitalWrite(sensor.ELECTROVALVULA, LOW);
    //Serial.println("Abrir la valvula manual.");
    while (aux != 'a' and Valvula_manual == false)
    {
        caudal.Valores();
        while (Serial.available() > 0)
        {
            aux = Serial.read();
            if (aux == 'a')
            {
                Valvula_manual = false;
                ejecucion = true;
                //if (not(pass_f))
                    //Serial.println("Valvula Manual: ON.");
                digitalWrite(sensor.VALVULA_MANUAL, HIGH);
                break;
            }
            else if (aux == '5')
            {
                return aux;
            }
        }
    }
    aux = '\0';
    return aux;
}

bool Functional::estado_90(bool array[5])
{
    if (not(Valvula_manual))
    {
        digitalWrite(sensor.ELECTROVALVULA, HIGH);
        //Serial.println("Fallo el proceso en la fase 3.");
        delay(2000);
        //Serial.println("Cierre de emergencia de la eletrovalvula.");
        //Serial.println("Nivel del 90%: Alcanzado.");
        digitalWrite(sensor.CONUMTADOR, LOW);
        digitalWrite(sensor.ELECTROVALVULA, LOW);
        digitalWrite(sensor.ALARMA, HIGH);
        if (conmutador)
        {
            array[4] = true;
            abrir_valvula();
            //Serial.println("Nivel descendiendo.");
            while (array[4])
            {
                caudal.Valores();
                while (Serial.available() > 0)
                {
                    aux = (char)Serial.read();
                    if (aux == '4')
                    {
                        array[4] = false;
                        pass_f = true;
                        exceso = false;
                        digitalWrite(sensor.ALARMA, LOW);
                        digitalWrite(sensor.CONUMTADOR, HIGH);
                        return true;
                    }
                    if (aux == 't')
                    {
                        //Serial.println("Se ha detenido el proceso. Conmutador apagado.");
                        exceso = false;
                        pass_f = false;
                        conmutador = false;
                        ejecucion = false;
                        array[4] = false;
                        delay(5000);
                        //Serial.println("Cerrar Valvula Manual.");
                        while (Valvula_manual)
                            cerrar_valvula(array);
                    }
                }
            }
        }
    }
    else
    {
        //Serial.println("Error Critico.");
        conmutador = false;
        return false;
    }
    return false;
}

bool Functional::terminar_proceso(bool array[5], char &aux)
{
    if (aux == 't')
    {
        Serial.println("Se ha detenido el proceso");
        digitalWrite(10, LOW);
        delay(10000);
        Serial.println("Tanque vaceado");
        digitalWrite(12, LOW);
        Serial.println("Fin de la jornada. Que tenga buen dia");
        aux = '\0';
        conmutador = false;
        for (int i = 0; i < 5; i++)
            array[i] = false;
        return false;
    }
    return true;
}

bool Functional::estado_20(bool array[5])
{
    if (not(array[4]) && not(array[3]) && not(array[1]) && not(array[2]))
    {
        Serial.println("Nivel del 20%: Alcanzado.");
        array[0] = false;
        Valvula_manual = false;
        Serial.readString();
        Serial.println("Cerrar Valvula manual.");
        while (cerrar_valvula(array))
            caudal.Valores();
        digitalWrite(13, HIGH);
        if (conmutador)
            Serial.println("Se abrio la eletrovalvula");
        pass_f = false;
        return false;
    }
    else
    {
        ejecucion = false;
        Valvula_manual = false;
        digitalWrite(sensor.ELECTROVALVULA, LOW);
        return false;
    }
}

char &Functional::Sensores_estado_6(bool array[5], bool descender)
{
    if (conmutador)
    {
        if (not(pass_f))
            aux = abrir_valvula();
        if (aux != '5')
            Serial.println("Eletrovalvula: OFF");
        if (aux != '5')
            Serial.println("Descendiendo.");
        if (aux != '5')
            llenado_nivel(array, descender);
    }
    return aux;
}

void Functional::llenado_nivel(bool array[5], bool descender)
{
    bool value{false};
    estados state = DESACTIVADO;
    while (ejecucion)
    {
        caudal.Valores();
        // Bucle de desición de activacion de sensores:
        while (Serial.available() > 0)
        {
            aux = (char)Serial.read();
            if (state == DESACTIVADO)
                state = aux == '5' ? ESTADO90 : DESACTIVADO;
            if (state == DESACTIVADO)
                state = aux == '4' ? ESTADO80 : DESACTIVADO;
            if (state == DESACTIVADO)
                state = aux == '3' ? ESTADO60 : DESACTIVADO;
            if (state == DESACTIVADO)
                state = aux == '2' ? ESTADO40 : DESACTIVADO;
            if (state == DESACTIVADO)
                state = aux == '1' ? ESTADO20 : DESACTIVADO;
            // Bucle principal de ejecución.
            if (state == ESTADO90 || exceso)
            {
                if (array[3] && array[0] && array[1] && array[2])
                {
                    aux = '\0';
                    descender = estado_90(array);
                }
            }
            else if (state == ESTADO80 || pass_f)
            {
                if (not(array[4]) && array[0] && array[1] && array[2] && not(descender))
                {
                    if (not(pass_f))
                        array[3] = true;
                    if (not(pass_f))
                        Serial.println("Nivel del 80%: Superado.");
                    while ('4' == aux or pass_f)
                    {
                        caudal.Valores();
                        aux = Sensores_estado_6(array, true);
                        if (aux == '5')
                        {
                            exceso = true;
                        }
                    }
                }
                else if (not(array[4]) && array[0] && array[1] && array[2] && descender)
                {
                    //Serial.println("Nivel del 80%: Alcanzado.");
                    array[3] = false;
                    pass_f = false;
                }
                else
                    ejecucion = false;
            }
            else if (state == ESTADO60)
            {
                if (not(array[4]) && array[0] && array[1] && not(array[3]) && not(descender))
                {
                    //Serial.println("Nivel del 60%: superado.");
                    array[2] = true;
                }
                else if (not(array[4]) && array[0] && array[1] && not(array[3]) && descender)
                {
                    //Serial.println("Nivel del 60%: Alcanzado.");
                    array[2] = false;
                }
                else
                    ejecucion = false;
            }
            else if (state == ESTADO40)
            {
                if (not(array[4]) && array[0] && not(array[2]) && not(array[3]) && not(descender))
                {
                    //Serial.println("Nivel del 40%: superado.");
                    array[1] = true;
                }
                else if (not(array[4]) && array[0] && not(array[2]) && not(array[3]) && descender)
                {
                    //Serial.println("Nivel del 40%: Alcanzado.");
                    array[1] = false;
                }
                else
                    ejecucion = false;
            }
            else if (state == ESTADO20)
            {
                if (not(array[4]) && not(array[1]) && not(array[2]) && not(array[3]) && not(descender))
                {
                    //Serial.println("Nivel del 20%: superado.");
                    array[0] = true;
                }
                else if (not(array[4]) && not(array[3]) && not(array[1]) && not(array[2]) && descender)
                {
                    descender = estado_20(array);
                }
                else
                    ejecucion = false;
            }
            else if (aux == 't' && not(descender))
            {
                conmutador = false;
                //Serial.println("Se ha detenido el proceso. Conmutador apagado.");
                digitalWrite(sensor.ELECTROVALVULA, LOW);
                digitalWrite(sensor.CONUMTADOR, LOW);
                ejecucion = false;
            }
            else if (aux == 't' && descender)
                ejecucion = terminar_proceso(array, aux);
            state = DESACTIVADO;
        }
        //Bucle de deteción de errores:
        if (ejecucion == false && not(descender) && conmutador)
        {
            //Serial.println("Error: Disminucion");
            if(value == true)
            {
                digitalWrite(sensor.ELECTROVALVULA, LOW);
                digitalWrite(sensor.CONUMTADOR, LOW);
                conmutador = false;
                ejecucion = false;
                value = false;
            }
            else
            {
                digitalWrite(sensor.CONUMTADOR, LOW);
                delay(3000);
                digitalWrite(sensor.CONUMTADOR, HIGH);
                ejecucion = true;
            }
            value = true;
        }
        else if (ejecucion == false && (descender) && conmutador)
        {
            //Serial.println("Error: Aumento");
            if (value == true)
            {
                conmutador = false;
                digitalWrite(sensor.ELECTROVALVULA, LOW);
                digitalWrite(sensor.CONUMTADOR, LOW);
                for (int i = 0; i < 5; i++)
                    array[i] = false;
                //Serial.println("Tanque vaceado");
                ejecucion = false;
                value = false;
            }
            else
            {
                digitalWrite(sensor.CONUMTADOR, LOW);
                delay(3000);
                digitalWrite(sensor.CONUMTADOR, HIGH);
                ejecucion = true;
            }
            value = true;
        }
    }
}

void Functional::Encender(bool array[5])
{
    if (ejecucion)
        Serial.println("Se prendio el tanque.");
    digitalWrite(sensor.CONUMTADOR, HIGH);
    vericacion_vaciado(array);
}
/*
void Functional::Regresion(Regresion_Cuadratica &Matriz)
{
    int contador_num{0};
    // Bucle principal de toma de datos:
    float aux{};
    do
    {
        delay(1000);
        aux = contador_num > 0 ? Celda_Carga.Valores.y[contador_num - 1] : 0;
        contador_num++;
        // Si se ha realizado la toma de valores con un patron incapaz de ser acomodados en una curva cuadratica. Lazara error.
        if (Matriz.get_Realizar())
        {
            Serial.print(contador_num);
        }
    } while (Matriz.Update(Celda_Carga.Valores.y[contador_num - 1]));
    balanza.set_scale(Celda_Carga.Valores.suma_valores);
    do
    {
        if (digitalRead(static_cast<uint8_t>(_sensor_::Sensor5)))
        {
            Serial.println("Se ha estableciod el valor # 5");
            Matriz.Update_x(balanza.get_units(20));
            Calibrar.velocidad = ((Celda_Carga.Valores.peso_total * Celda_Carga.densidad) / 9.8) / Calibrar.time;
            break;
        }
        else if (digitalRead(static_cast<uint8_t>(_sensor_::Sensor4)))
        {
            Serial.println("Se ha estableciod el valor # 4");
            Matriz.Update_x(balanza.get_units(20));
        }
        else if (digitalRead(static_cast<uint8_t>(_sensor_::Sensor3)))
        {
            Serial.println("Se ha estableciod el valor # 3");
            Matriz.Update_x(balanza.get_units(20));
        }
        else if (digitalRead(static_cast<uint8_t>(_sensor_::Sensor2)))
        {
            Serial.println("Se ha estableciod el valor # 2");
            Matriz.Update_x(balanza.get_units(20));
        }
        else if (digitalRead(static_cast<uint8_t>(_sensor_::Sensor1)))
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
*/