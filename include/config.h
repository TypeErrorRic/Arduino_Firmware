#ifndef CELDA_CARGA_CONFIG_H
#define CELDA_CARGA_CONFIG_H

#include <Memoria_no_volatil.h>

#define INIZIALIZAR() EEPROM.read((EEPROM.length(), 0))

struct Elementos_Memoria
{
    Memoria_no_volatil Memoria{0};
    int numero;
};

#endif