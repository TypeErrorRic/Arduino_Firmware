#include <Functional.h>

sensores Functional::sensor{};
bool Functional::conmutar = false;

void Functional::init(float array[3], float &altura)
{
    //Inicialicializando puertos:
    pinMode(sensor.ELECTROVALVULA, OUTPUT);
    pinMode(sensor.VALVULA_MANUAL, OUTPUT);
    pinMode(sensor.ALARMA, OUTPUT);
    pinMode(sensor.CONUMTADOR, INPUT);
    //Inicializando estado de los puertos:
    digitalWrite(sensor.ELECTROVALVULA, LOW);
    digitalWrite(sensor.VALVULA_MANUAL, LOW);
    digitalWrite(sensor.ALARMA, LOW);
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
        Variables::Lcd.setCursor(0, 0);
        Variables::Lcd.print("Nivel del 90%.");
        //Serial.println("Nivel del 90%: Alcanzado.");
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
                while (caudal.lectura_values() > 0)
                {
                    aux = (char)caudal.return_value_nivel();
                    if (aux == '4')
                    {
                        array[4] = false;
                        pass_f = true;
                        exceso = false;
                        digitalWrite(sensor.ALARMA, LOW);
                        digitalWrite(sensor.CONUMTADOR, HIGH);
                        return true;
                    }
                    if (digitalRead(sensor.CONUMTADOR) == 0)
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
    if (digitalRead(sensor.CONUMTADOR) == LOW)
    {
        digitalWrite(10, LOW);
        //Verficar 20%
        Variables::Lcd.print("Tanque vaceado");
        digitalWrite(12, LOW);
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
        Variables::Lcd.setCursor(0, 0);
        Variables::Lcd.print("Nivel 20%:");
        array[0] = false;
        Valvula_manual = false;
        Variables::Lcd.print("Cerrar Valvula manual");
        while (cerrar_valvula(array))
            caudal.Valores();
        digitalWrite(13, HIGH);
        if (conmutador)
        {
            Variables::Lcd.setCursor(0, 0);
            Variables::Lcd.println("Eletrovalvula ON");
        }
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
        {
            Variables::Lcd.setCursor(0, 0);
            Variables::Lcd.print("Eletrovalvula: OFF");
            Variables::Lcd.setCursor(0, 1);
            Variables::Lcd.print("Descendiendo.");
            llenado_nivel(array, descender);
        }
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
        //Comprobar estado del conmutador:
        if (digitalRead(10) == 0 && not(descender))
        {
            ejecucion = false;
            conmutador = false;
            digitalWrite(sensor.ELECTROVALVULA, LOW);
            value = false;
            Variables::Lcd.clear();
            Variables::Lcd.setCursor(0, 0);
            Variables::Lcd.print("Conmutador Off");
            delay(1000);
            Variables::Lcd.clear();
            break;
        }
        // Bucle de desición de activacion de sensores:
        while (caudal.lectura_values() > 0)
        {
            aux = (char)caudal.return_value_nivel();
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
            else if (state == ESTADO90 || exceso)
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
                    {
                        array[3] = true;
                        Variables::Lcd.setCursor(0, 0);
                        Variables::Lcd.print("Nivel del 80%");
                    }
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
                    Variables::Lcd.setCursor(0, 0);
                    Variables::Lcd.print("Nivel del 80%");
                    array[3] = false;
                    pass_f = false;
                }
                else
                    ejecucion = false;
            }
            else if (state == ESTADO60)
            {
                if (not(array[4]) && array[0] && array[1] && not(array[3]))
                {
                    Variables::Lcd.setCursor(0, 0);
                    Variables::Lcd.print("Nivel del 60%");
                    if (not(descender))
                        array[2] = true;
                    else
                        array[2] = false;
                }
                else
                    ejecucion = false;
            }
            else if (state == ESTADO40)
            {
                if (not(array[4]) && array[0] && not(array[2]) && not(array[3]) && not(descender))
                {
                    Variables::Lcd.setCursor(0, 0);
                    Variables::Lcd.print("Nivel del 40%");
                    if(not(descender))
                        array[1] = true;
                    else
                        array[1] = false;
                }
                else
                    ejecucion = false;
            }
            else if (state == ESTADO20)
            {
                if (not(array[4]) && not(array[1]) && not(array[2]) && not(array[3]) && not(descender))
                {
                    Variables::Lcd.setCursor(0, 0);
                    Variables::Lcd.print("Nivel del 20%");
                    if(not(descender))
                        array[0] = true;
                    else
                        descender = estado_20(array);
                }
                else
                    ejecucion = false;
            }
            else if ((digitalRead(10) == 0) && descender)
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
                for (int i = 0; i < 5; i++)
                    array[i] = false;
                //Serial.println("Tanque vaceado");
                ejecucion = false;
                value = false;
            }
            else
            {
                //digitalWrite(sensor.CONUMTADOR, LOW);
                delay(3000);
                //digitalWrite(sensor.CONUMTADOR, HIGH);
                ejecucion = true;
            }
            value = true;
        }
    }
}

void Functional::Encender(bool array[5])
{
    if (ejecucion && digitalRead(sensor.CONUMTADOR) == HIGH)
    {
        conmutar = true;
    }
    vericacion_vaciado(array);
}

void Functional::Encendido(bool value)
{
    conmutar = value;
}