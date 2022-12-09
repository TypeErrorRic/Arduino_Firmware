//Librerías:
#include <Arduino.h>
#include <Regresion_Cuadratica.h> 
#include <Caudal.h>
//Elementos de ejecuión:
#include "..\include\Set_up.h"
#include "..\include\Correr.h"

/**
 * @file Proyecto de Taller de Ingeniería.
 * @author Ricardo Pabón Serna.(ricardo.pabon@correounivalle.edu.co)
 * @brief Este programa se encarga de realizar el ajuste de un grupo de pares ordenados en una
 *        una curva cuadratica, devolviendo los parametros de la curva para su correspondiente análisis.
 * @version 0.1
 * @date 2022-10-17 
 *
 * @copyright Copyright (c) 2022
 */


/**
 * @brief Función que se encarga de configurar los parametros inciales de las funciones.
 */
void setup() 
{
  Correr::setup();
}


/**
 * @brief Bucle incial de ejecución.
 */
void loop() 
{
  Correr::loop();
}

/**
 * @note Para la comprobación de los calculos realizador por el programa <<Regresion_Cuadratica.h>
 *       Debe descomentar lqas funcionalidades de test y Get_Matriz.
 * @attention Cualquier inquietud informar al propietario del código.
 */