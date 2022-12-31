#ifndef TESTEO_H
#define TESTEO_H

/**
 * @file Testeo.h
 * @author Ricardo Pabón Serna (ricardo.pabon@correounivalle.edu.co)
 * @brief Funciones y variables que encargan de comprobar el funcionamiento del dispositivo.
 * @version 0.1
 * @date 2022-10-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>
#include "Config.h"

static constexpr unsigned int Nums{5};

// Testeo:
namespace Lectura_data
{
    //Banco de Pruebas:
    //Variables:
    const float x[Nums]{-2, 1, 4, 3, 0};
    const float y[Nums]{5, 4, 1, -1, 0};
    const short buffer{20}; //Capacidad maxima de almacenamiento.
    //Funciones:
    int Lectura_events_numerics(float& data);
    char *Asignacion_strings();
    void Captura_eventos();
}
#endif