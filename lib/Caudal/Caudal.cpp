#include "Caudal.h"

const float Caudal::Diametro = 4;
const float Caudal::Gravedad{9.8};

Caudal::Caudal(const float densidad,const float radio): 
  densidad(densidad), radio(radio), valores(nullptr)
{
  for(int i=2; i>= 0; i--)
    {
      derivada[i] = valores[i]*i;
    }
}

float Caudal::posicion(float &peso)
{
  return peso == 0 ? 0 : (valores[0]*(peso*peso))+(valores[1]*peso)+valores[2];  
}

float Caudal::volumen(float &peso)
{
  return (densidad*peso)/Gravedad;
}

void Caudal::set_valores(float valores[3])
{
  this->valores = &valores[0];
}
