#include "Caudal.h"

//Configuraciones:
static Variables_config config;
//instanciado la celda de carga.
HX711 Caudal::balanza;

//Constructor:
Caudal::Caudal(const float &densidad, const float &peso, Variables_Guardar &variables, const float &teoric_caudal) : 
  densidad(densidad), valores(nullptr), Peso_total(peso), value_caudal(teoric_caudal), variables(variables)
{
  this->caudal = 0;
  this->tiempo = millis();
  this->tanq_altura = 0;
  this->advertencia = false;
  this->Peso_ultimo = 0;
}

//Calculo del volumen.
float Caudal::volumen(float &peso)
{
  return (densidad * peso) / GRAVEDAD;
}

//Inicializa la celdad de carga.
void Caudal::set_valores(float valores[3], float &altura)
{
  this->valores = &valores[0];
  this->tanq_altura = altura;
  //Obtener valores de la EEPROM
  //Serial.println("Starting...");
  balanza.begin(config.HX711_dout, config.HX711_sck);
  Serial.println(balanza.read());
  balanza.set_scale(variables.suma_valores); // Establecemos la escala
  balanza.set_offset(variables.t); // El peso actual es considerado Tara.
}

//Muestra si el tanque se está vaceando o no.
int Caudal::estado(float &peso)
{
  Peso_ultimo = Peso_ultimo < peso ? peso : Peso_ultimo;
  return (Peso_ultimo - 0.2) < peso ? 1 : -1;
}

//Muestre el volumen y el porcentaje del taque lleno.
bool& Caudal::Valores()
{
  float peso = delta_caudal();
  peso = peso >= 0 ? peso : 0;
  Serial.print("Volumen: ");
  Serial.print(volumen(peso));
  Serial.print(" %Llenado: ");
  Serial.println((altura(peso) / tanq_altura) * 100);
  switch (estado(peso))
  {
  case -1:
    if (not((variacion_peso(delta_caudal()) && estado(peso) == -1)))
      advertencia = true;
    break;
  case 1:
    if (variacion_peso(delta_caudal()) && estado(peso) == 1)
      advertencia = true;
  default:
    Serial.println("Error: Inestable.");
    break;
  }
  return advertencia;
}

//Variación del caudal de vaceado cuando se da apertura a la valvula manual.
bool Caudal::variacion_peso(float& caudal)
{
  return caudal / value_caudal <= 0.1 ? true : false;
}

//Derivada de la señal de peso.
float& Caudal::delta_caudal()
{
  int valor{0};
  unsigned long time{0};
  bool continua{true};
  tiempo = millis();
  int out = Cal_peso();
  // Realizar derivada:
  do
  {
    if(time != tiempo && time != 0 && valor != 0)
    { 
      caudal = (valor-out);
      caudal /= (tiempo - time);
      continua = false;
    }
    time = millis();
    valor = Cal_peso();
  }
  while (continua);
  // Imprimir derivada:
  Serial.println(caudal);
  // Retorna el resultado:
  return caudal;
}

//Cálculo de la altura en función del peso.
float Caudal::altura(float &peso)
{
  if(valores != nullptr)
  {
    return (valores[0]*(peso)*(peso)) + valores[1]*peso + valores[0];
  }
  return -1;
}

//Cálculo de la variación de la altura en función del valor teorico.
bool Caudal::variacion_altura(float altura, float porcentaje)
{
  return ((altura/tanq_altura) * 100)/porcentaje < 0.2 ? true: false;
}

//Toma de valores de la celadad de carga.
float Caudal::Cal_peso()
{
  Serial.print(balanza.get_units(20), 3);
  delay(500);
  return balanza.get_units(20);
}

//Configura los valores inciales para calibrar la celdad de carga.
bool Caudal::calibracion_escala(long &offset, float &sum, bool& realizar)
{
  if (offset == 0)
  {
    balanza.begin(config.HX711_dout, config.HX711_sck);
    Serial.println(balanza.read());
    balanza.set_scale(); // La escala por defecto es 1
    balanza.tare(20);    // El peso actual es considerado Tara.
    offset = balanza.get_offset();
  }
  else
  {
    if(realizar)
    {
      float array[10] = {};
      for (short i = 0; i < 10; i++)
      {
        array[i % 10] = balanza.get_value(10);
        sum += array[i % 10];
        delay(100);
      }
      return true;
    }
    Serial.println(balanza.get_value(10));
  }
  return false;
}