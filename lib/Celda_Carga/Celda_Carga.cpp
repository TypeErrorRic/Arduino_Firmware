/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

#include <Celda_Carga.h>
#include <EEPROM.h>

/**
 * @brief Construct a new Celda_Carga::Celda_Carga object
 * 
 * @param Port_conect 
 * @param Port_out 
 */
Celda_Carga::Celda_Carga(const short Port_conect, const short Port_out, bool _tare) : 
    Balanza(Port_conect, Port_out), stabilizingtime{0}, calibracion()
{
    this -> _tare = _tare;
    Balanza.begin();
    Balanza.start(stabilizingtime, _tare);
    if (Balanza.getTareTimeoutFlag() || Balanza.getSignalTimeoutFlag())
    {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        while (1);
    }
    else
    {
        Balanza.setCalFactor(1.0); // user set calibration value (float), initial value 1.0 may be used for this sketch
        Serial.println("Startup is complete");
    }
    this -> resume = true;
}


/**
 * @brief Calibra la celda de carga:
 * 
 */
void Celda_Carga::Calibracion_peso()
{
    float known_mass = 0;
    resume = false;
    while (resume == false)
    {
        Balanza.update();
        if (Serial.available() > 0)
        {
            known_mass = Serial.parseFloat();
            if (known_mass != 0)
            {
                Serial.print("Known mass is: ");
                Serial.println(known_mass);
                resume = true;
            }
        }
    }

    Balanza.refreshDataSet();                                          // refresh the dataset to be sure that the known mass is measured correct
    float newCalibrationValue = Balanza.getNewCalibration(known_mass); // get the new calibration value

    Serial.print("New calibration value has been set to: ");
    Serial.print(newCalibrationValue);
    Serial.println(", use this as calibration value (calFactor) in your project sketch.");
    Serial.print("Save this value to EEPROM adress ");
    Serial.print(calibracion.get_Adress());
    Serial.println("? y/n");

    resume = false;
    while (resume == false)
    {
        if (Serial.available() > 0)
        {
            char inByte = Serial.read();
            if (inByte == 'y')
            {
                calibracion.Escritura<float>(newCalibrationValue);
                calibracion.Lectura<float>(newCalibrationValue);
                Serial.print("Value ");
                Serial.print(newCalibrationValue);
                Serial.print(" saved to EEPROM address: ");
                Serial.println(calibracion.get_Adress());
                resume = true;
            }
            else if (inByte == 'n')
            {
                Serial.println("Value not saved to EEPROM");
                resume = true;
            }
        }
    }

    Serial.println("Fin de la calibración");
    Serial.println("***");
    Serial.println("To re-calibrate, send 'r' from serial monitor.");
    Serial.println("For manual edit of the calibration value, send 'c' from serial monitor.");
    Serial.println("***");
}


/**
 * @brief 
 * 
 */
void Celda_Carga::changeSavedCalFactor()
{
    float oldCalibrationValue = Balanza.getCalFactor();
    boolean _resume = false;
    Serial.println("***");
    Serial.print("Current value is: ");
    Serial.println(oldCalibrationValue);
    Serial.println("Now, send the new value from serial monitor, i.e. 696.0");
    float newCalibrationValue;
    while (_resume == false)
    {
        if (Serial.available() > 0)
        {
            newCalibrationValue = Serial.parseFloat();
            if (newCalibrationValue != 0)
            {
                Serial.print("New calibration value is: ");
                Serial.println(newCalibrationValue);
                Balanza.setCalFactor(newCalibrationValue);
                _resume = true;
            }
        }
    }
    _resume = false;
    Serial.print("Save this value to EEPROM adress ");
    Serial.print(calibracion.get_Adress());
    Serial.println("? y/n");
    while (_resume == false)
    {
        if (Serial.available() > 0)
        {
            char inByte = Serial.read();
            if (inByte == 'y')
            {
                calibracion.Escritura<float>(newCalibrationValue);
                calibracion.Lectura<float>(newCalibrationValue);
                Serial.print("Value ");
                Serial.print(newCalibrationValue);
                Serial.print(" saved to EEPROM address: ");
                Serial.println(calibracion.get_Adress());
                _resume = true;
            }
            else if (inByte == 'n')
            {
                Serial.println("Value not saved to EEPROM");
                _resume = true;
            }
        }
    }
    Serial.println("End change calibration value");
    Serial.println("***");
}


void Celda_Carga::Update()
{
    Serial.println("**************************************");
    Serial.println("Send 't' from serial monitor to set the tare offset.");
    while(!Balanza.update())
    {
        if(Serial.available() > 0)
        {
            Configuracion_tare();
            Calibracion_peso();
        }
    }
}

void Celda_Carga::Configuracion_tare()
{
    resume = false;
    while (resume == false)
    {
        Balanza.update();
        if (Serial.available() > 0){
            char inByte = Serial.read();
            if (inByte == 't')
            Serial.println("Iniciando la calibración:");
            Serial.println("Asegurate de que la balanza este colocada en una superficie estable.");
            delay(5000);
            Serial.println("Remueve cualquier peso que se encutre encima de la Superficie:");
            Balanza.tareNoDelay();
        }
        if (Balanza.getTareStatus() == true)
        {
            Serial.println("Tare complete");
            resume = true;
        }
    }

    Serial.println("Now, place your known mass on the loadcell.");
    Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");
}