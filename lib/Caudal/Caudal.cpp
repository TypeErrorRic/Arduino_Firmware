#include "Caudal.h"

// Configuraciones:
static Variables_config config;
// instanciado la celda de carga.
HX711 Caudal::balanza;
// Instanciado Pantalla:
static LiquidCrystal LCD(7, 6, 5, 4, 3, 2);

// Constructor:
Caudal::Caudal(const float &densidad, const float &peso, Variables_Guardar &variables, const float &teoric_caudal) : 
  densidad(densidad), valores(nullptr), Peso_total(peso), value_caudal(teoric_caudal),                                                                                                                    variables(variables), LCD(7, 6, 5, 4, 3, 2)
{
  this->caudal = 0;
  this->tiempo = millis();
  this->tanq_altura = 0;
  this->advertencia = false;
  this->Peso_ultimo = 0;
  this->peso_turno = 0;
  LCD.begin(16, 2);
  this->nivel = '0';
  this->confirmacion=false;
}

// Calculo del volumen.
float Caudal::volumen(float &peso)
{
  return (densidad * peso) / GRAVEDAD;
}

// Inicializa la celdad de carga.
void Caudal::set_valores(float valores[3], float &altura)
{
  this->valores = &valores[0];
  this->tanq_altura = altura;
  balanza.begin(config.HX711_dout, config.HX711_sck);
  Serial.println(balanza.read());
  balanza.set_scale(variables.suma_valores); // Establecemos la escala
  balanza.set_offset(variables.t);           // El peso actual es considerado Tara.
  pinMode(Sensor20, INPUT);
  pinMode(Sensor40, INPUT);
  pinMode(Sensor60, INPUT);
  pinMode(Sensor80, INPUT);
  pinMode(Sensor100, INPUT);
}

// Muestra si el tanque se está vaceando o no.
int Caudal::estado(float &peso)
{
  if ((Peso_ultimo - 0.3) > peso)
  {
    Peso_ultimo += Peso_ultimo;
  }
  Peso_ultimo = Peso_ultimo < peso ? peso : Peso_ultimo;
  return (Peso_ultimo - 0.2) < peso ? 1 : -1;
}

// Muestre el volumen y el porcentaje del taque lleno.
bool &Caudal::Valores()
{
  // float peso = delta_caudal();
  // peso = peso >= 0 ? peso : 0;
  // float nivel = (altura(peso) / tanq_altura) * 100;
  LCD.setCursor(0, 0);
  String aux = "V: " + String(1.2456 /*volumen(peso)*/, 2) + " C: " + String(2.345 /*peso, 3*/, 2);
  LCD.print(aux);
  LCD.setCursor(0, 1);
  String aux2 = "N: " + String(2 /*nivel, 3*/) + "% " + "T: " + String(10) + "l";
  LCD.print(aux2);
  /*
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
  */
  return advertencia;
}

// Variación del caudal de vaceado cuando se da apertura a la valvula manual.
bool Caudal::variacion_peso(float &caudal)
{
  return caudal / value_caudal <= 0.1 ? true : false;
}

// Derivada de la señal de peso.
float &Caudal::delta_caudal()
{
  int valor{0};
  unsigned long time{0};
  bool continua{true};
  tiempo = millis();
  int out = Cal_peso();
  // Realizar derivada:
  do
  {
    if (time != tiempo && time != 0 && valor != 0)
    {
      caudal = (valor - out);
      caudal /= (tiempo - time);
      continua = false;
    }
    time = millis();
    valor = Cal_peso();
    delay(100);
  } while (continua);
  // Imprimir derivada:
  Serial.println(caudal);
  // Retorna el resultado:
  return caudal;
}

// Cálculo de la altura en función del peso.
float Caudal::altura(float &peso)
{
  if (valores != nullptr)
  {
    return (valores[2] * (peso) * (peso)) + valores[1] * peso + valores[0];
  }
  return -1;
}

// Cálculo de la variación de la altura en función del valor teorico.
bool Caudal::variacion_altura(float altura, float porcentaje)
{
  return ((altura / tanq_altura) * 100) / porcentaje < 0.2 ? true : false;
}

// Toma de valores de la celadad de carga.
float Caudal::Cal_peso()
{
  Serial.print(balanza.get_units(20), 3);
  delay(500);
  return balanza.get_units(20);
}

// Configura los valores inciales para calibrar la celdad de carga.
bool Caudal::calibracion_escala(long &offset, float &sum, bool &realizar)
{
  if (offset == 0)
  {
    balanza.begin(config.HX711_dout, config.HX711_sck);
    // balanza.read();
    balanza.set_scale(); // La escala por defecto es 1
    balanza.tare(20);    // El peso actual es considerado Tara.
    offset = balanza.get_offset();
  }
  else
  {
    if (realizar)
    {
      sum = 0;
      Variables::Lcd.clear();
      Variables::Lcd.setCursor(0, 0);
      Variables::Lcd.print("Tomando");
      for (short i = 0; i < 10; i++)
      {
        sum += balanza.get_value(10);
        delay(100);
      }
      sum /= 10;
      return true;
    }
    Variables::Lcd.setCursor(0, 0);
    Variables::Lcd.print("Valor: ");
    Variables::Lcd.print(balanza.get_value(10));
    delay(100);
  }
  return false;
}

int Caudal::lectura_values()
{
  if (digitalRead(Sensor20) == LOW && digitalRead(Sensor40) == HIGH && nivel != '1')
  {
    if (confirmacion)
    {
      nivel = '1';
      confirmacion = false;
      return 1;
    }
    else
    {
      confirmacion = false;
      delay(500);
    }
  }
  else if (digitalRead(Sensor40) == LOW && digitalRead(Sensor60) == HIGH && nivel != '2')
  {
    if (confirmacion)
    {
      nivel = '2';
      confirmacion = false;
      return 2;
    }
    else
    {
      confirmacion = false;
      delay(500);
    }
  }
  else if (digitalRead(Sensor60) == LOW && digitalRead(Sensor80) == HIGH && nivel != '3')
  {
    if (confirmacion)
    {
      nivel = '3';
      confirmacion = false;
      return 3;
    }
    else
    {
      confirmacion = false;
      delay(500);
    }
  }
  else if (digitalRead(Sensor80) == LOW && digitalRead(Sensor100) == HIGH && nivel != '4')
  {
    if(confirmacion)
    {
      nivel = '4';
      confirmacion = false;
      return 4;
    }
    else
    {
      confirmacion = true;
      delay(500);
    }
  }
  else if (digitalRead(Sensor100) == LOW && digitalRead(Sensor80) == HIGH && nivel != '5')
  {
    if(confirmacion)
    {
      nivel = '5';
      confirmacion = false;
      return 5;
    }
    else
    {
      confirmacion = true;
      delay(500);
    }
  }
  else
  {
    confirmacion = false;
  }
  return 0;
}

char Caudal::return_value_nivel()
{
  return nivel;
}

void Caudal::Calibracion_niveles(float array[5])
{
  bool continuar{true};
  LCD.setCursor(0, 0);
  LCD.print("Peso: ");
  LCD.print(Cal_peso());
  while (continuar)
  {
    while (lectura_values() > 0)
      switch (nivel)
      {
      case '1':
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("#1 tomado");
        array[0] = Cal_peso();
        delay(1000);
        LCD.clear();
        break;
      case '2':
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("#2 tomado");
        array[1] = Cal_peso();
        delay(1000);
        LCD.clear();
        break;
      case '3':
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("#3 tomado");
        array[2] = Cal_peso();
        delay(1000);
        LCD.clear();
        break;
      case '4':
        break;
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("#4 tomado");
        array[3] = Cal_peso();
        delay(1000);
        LCD.clear();
      case '5':
        LCD.clear();
        LCD.setCursor(0, 0);
        array[4] = Cal_peso();
        LCD.clear();
        continuar = false;
        break;
      default:
        break;
      }
  }
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Medidas Tomadas");
}