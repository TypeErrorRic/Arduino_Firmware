#include "Caudal.h"

#define pi 3.14

Caudal::Caudal(const float densidad, const float peso, const float &radio) : 
densidad(densidad), valores(nullptr), Peso_total(peso), radio(radio)
{
  for (int i = 0; i <= 3; i++) velocidad[i] = valores[i] * (2-i);
  this -> velocidad_caudal = 0;
  this->Peso_ultimo = 1;
  this->tiempo = millis();
}

float Caudal::volumen(float &peso)
{
  return (densidad*peso)/velocidad[0];
}

void Caudal::set_valores(float valores[3], float velocidad)
{
  this->velocidad_caudal = velocidad;
  this->valores = &valores[0];
}

float Caudal::estado(float& peso)
{
  Peso_ultimo = Peso_ultimo < peso ? peso: Peso_ultimo;
  return (Peso_ultimo-0.2) < peso ? 1 : -1;
}

float Caudal::Valores(float & peso)
{
  peso = peso >= 0 ? peso : 0;
  if(variacion_peso(volumen(peso)))
  {
    Serial.print("Volumen en el tanque: ");
    Serial.print(volumen(peso));
    Serial.print("            Porcentaje del tanque lleno: ");
    Serial.println(volumen(peso) / Peso_total);
  }
  else Serial.println("Fluo de agua no constante reiniciar.");
}

bool Caudal::variacion_peso(float volumen)
{
  unsigned long tiempo1 = millis();
  bool aux;
  if((tiempo1 - tiempo) >= 1000)
  {
    aux = (((volumen - volumen_ultimo)/(tiempo1 -tiempo))/(2*pi*radio))/velocidad_caudal <= 0.1 ? true : false;
    tiempo = millis();
    return aux;
  }
  else return true;
}