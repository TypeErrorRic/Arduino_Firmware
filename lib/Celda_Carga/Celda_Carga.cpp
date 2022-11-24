#include <Celda_Carga.h>
#include <EEPROM.h>


/**
 * @brief Construct a new Celda_Carga::Celda_Carga object
 * 
 * @param Port_conect 
 * @param Port_out 
 */
Celda_Carga::Celda_Carga(const short Port_conect, const short Port_out) : 
    Balanza(Port_conect, Port_out), calVal_eepromAdress{0}
{

}


/**
 * @brief Calibra la celda de carga:
 * 
 */
void Celda_Carga::Calibracion()
{
    Serial.println("***");
    Serial.println("Start calibration:");
    Serial.println("Send 't' from serial monitor to set the tare offset.");

    boolean _resume = false;
    while (_resume == false)
    {
        Balanza.update();
        if (Serial.available() > 0)
        {
            if (Serial.available() > 0)
            {
                char inByte = Serial.read();
                if (inByte == 't')
                    Balanza.tareNoDelay();
            }
        }
        if (Balanza.getTareStatus() == true)
        {
            Serial.println("Tare complete");
            _resume = true;
        }
    }

    float known_mass = 0;
    _resume = false;
    while (_resume == false)
    {
        Balanza.update();
        if (Serial.available() > 0)
        {
            known_mass = Serial.parseFloat();
            if (known_mass != 0)
            {
                Serial.print("Known mass is: ");
                Serial.println(known_mass);
                _resume = true;
            }
        }
    }

    Balanza.refreshDataSet();                                          // refresh the dataset to be sure that the known mass is measured correct
    float newCalibrationValue = Balanza.getNewCalibration(known_mass); // get the new calibration value

    Serial.print("New calibration value has been set to: ");
    Serial.print(newCalibrationValue);
    Serial.println(", use this as calibration value (calFactor) in your project sketch.");
    Serial.print("Save this value to EEPROM adress ");
    Serial.print(calVal_eepromAdress);
    Serial.println("? y/n");

    _resume = false;
    while (_resume == false)
    {
        if (Serial.available() > 0)
        {
            char inByte = Serial.read();
            if (inByte == 'y')
            {
#if defined(ESP8266) || defined(ESP32)
                EEPROM.begin(512);
#endif
                EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266) || defined(ESP32)
                EEPROM.commit();
#endif
                EEPROM.get(calVal_eepromAdress, newCalibrationValue);
                Serial.print("Value ");
                Serial.print(newCalibrationValue);
                Serial.print(" saved to EEPROM address: ");
                Serial.println(calVal_eepromAdress);
                _resume = true;
            }
            else if (inByte == 'n')
            {
                Serial.println("Value not saved to EEPROM");
                _resume = true;
            }
        }
    }

    Serial.println("End calibration");
    Serial.println("***");
    Serial.println("To re-calibrate, send 'r' from serial monitor.");
    Serial.println("For manual edit of the calibration value, send 'c' from serial monitor.");
    Serial.println("***");
}