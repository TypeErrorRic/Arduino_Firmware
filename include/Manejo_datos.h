#ifndef MANEJO_DATOS_H
#define MANEJO_DATOS_H

#include <Memoria_no_volatil.h>

namespace Manejo_datos
{
    void inicializar_default();
    void set_up();
    void Guardar_regresion(float array[3]);
    void Guardar_config_celda_carga(long &tara, float &suma);
    void guarda_config_peso(float &peso);
    void guarda_config_altura(float &altura);
    void guardar_peso_diario(float &dias);
    void limpiar_data();
    bool inicializar();
    Variables_Guardar &borrar_list();
    Variables_Guardar &get_datos();
};

#endif