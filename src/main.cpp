/**
 * @file Proyecto de Taller de Ingeniería.
 * @author Ricardo Pabón Serna.(ricardo.pabon@correounivalle.edu.co)
 * @brief Este programa se encarga de realizar el ajuste de un grupo de pares ordenados en una
 *        una curva cuadratica, devolviendo los parametros de la curva para su correspondiente análisis.
 * @version 0.1
 * @date 2022-10-17
 *
 *
 * @include {lib} Caudal.h
 * @include {lib} Functional.h
 * @include {lib} Memoria_no_volatil.h
 * @include {std} Arduino.h
 * @include {std} SoftwareSerial.h
 *
 *
 * @copyright Copyright (c) 2022
 */

//Elementos de ejecuión:
#include "..\include\Correr.h"
/**
 * @brief Función que se encarga de configurar los parametros inciales de las funciones.
 */

bool calibrar{true};

void setup() 
{
  Variables::Lcd.begin(16, 2);
  Serial.begin(Variables::VELOCIDAD_DATOS);
  Serial.println("H05ricF45");
  unsigned long time = millis();
  unsigned long tiempo2{0};
  bool continuar{false};
  while (calibrar)
  {
    tiempo2 = millis();
    Variables::Lcd.setCursor(0, 0);
    Variables::Lcd.print("Iniciando");
    if(tiempo2 <= (time+5000))
    {
      while (Serial.available())
      {
        calibrar = false;
        String cadena = Serial.readString();
        if(cadena == "HGTCh4rGu")
        {
          Serial.println(cadena);
          calibrar = true;
          continuar = true;
          break;
        }
        else
          break;
      }
      if(continuar)
        break;
      tiempo2 = millis();
    }
    else
    {
      calibrar = false;
      break;
    }
  }
  /*
  Manejo_datos::limpiar_data();
  Manejo_datos::set_up();
  float var{12.5};
  Manejo_datos::guardar_peso_diario(var);
  Manejo_datos::guardar_peso_diario(var);
  Manejo_datos::guardar_peso_diario(var);
  */
  Correr::setup();
  Correr::calibracion(calibrar);
}

/**
 * @brief Bucle incial de ejecución.
 */
void loop() 
{
  //Correr::loop();
  Variables::Lcd.setCursor(0,0);
  Variables::Lcd.print("Hola ricardo.");
}

/**
 * @note Para la comprobación de los calculos realizador por el programa <<Regresion_Cuadratica.h>
 *       Debe descomentar lqas funcionalidades de test y Get_Matriz.
 * @attention Cualquier inquietud informar al propietario del código.
 */