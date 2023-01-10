from module.Transfer import Transfer
from module.Base_datos import Almacenamiento

class Arduino(Transfer):

    Guardar : Almacenamiento = Almacenamiento()
    Datos_estadisticos : list  = []
    Diccionario_arduino : dict = {}

    def __new__(cls, baudios: int, listas: list = [], variable: str = ""):
        cls.port_list = listas
        return super().__new__(cls, baudios, variable)

    def __init__(self, baudios: int, listas: list = [], variable: str = ""):
        super().__init__(baudios, variable)
        self.__Regresion : dict = {}
        self.__contador2: int = 0
        self.Guardar.Cargar_datos()

    def datos_guardos_json(self) -> str:
        return self.Guardar.__str__()
    
    def __Enviar_valores_regresion(self, lista : list) -> None:
        aux : str = "["
        for element in lista:
            aux += "{:.2}/".format(element)
        aux = aux + "0]"
        print(aux)
        super().escribir_datos(aux)
        self.guardar_valores_regresion(lista)

    def Regresion(self, value : float):
        if (contador := self.__contador2 + 1) <= (3):
            if self.__contador2 == 0:
                self.__Regresion.setdefault("a", value)
            else:
                self.__Regresion.setdefault(f"x{contador}", value)
            self.__contador2 = contador
            if(self.__contador2 == 3):
                self.Guardar.datos_arduino = self.__Regresion
                self.__Enviar_valores_regresion(list(self.__Regresion.values()))
                self.Guardar.Guardar_datos()

    def confirmar_regresion(self) -> bool:
        if self.Arduino.is_open:
            if self.señal.isSet():
                count: int = 0 
                try:
                    if(len(self.data) == 3):
                        for indx, element in enumerate(self.data , start=0):
                            if(element == self.data[indx]):
                                count += 1
                        if count == 3:
                            self.limpiar()
                            return True
                        else:
                            self.limpiar()
                            return False
                    else:
                        return False
                except ValueError:
                    self.limpiar()
                    return False
            else:
                print("Hilo fallo")
                self.Arduino.close()
                return False
        else:
            return False

    def conection(self, value: str) -> bool:
        if self.Arduino.is_open:
            if self.señal.isSet():
                for intem in self.data:
                    try:
                        valu_aux: float = float(intem)
                        if valu_aux == float(value):
                            self.limpiar()
                            return True
                    except ValueError:
                        if intem == value:
                            self.limpiar()
                            return True
                return False
            else:
                print("Hilo fallo")
                self.Arduino.close()
                return False
        else:
            return False
    
    def mostrar_datos_en_buffer(self) -> None:
        if(len(self.__data) == 0):
            print("sin datos")
        else:
            for item in self.__data:
                print(item)

    def datos_estadisticos_ard(self, validar: bool) -> bool:
        if validar:
            if len(self.data) == 31:
                for indx, item in enumerate(self.data, start=1):
                    try:
                        if indx != 31:
                            self.Datos_estadisticos.append(float(item))
                        else:
                            self.Guardar.Posicion_dia = float(item)
                            if(self.Datos_estadisticos[29] != 0):
                                self.Datos_estadisticos = self.Datos_estadisticos[self.Guardar.Posicion_dia:] + self.Datos_estadisticos[:self.Guardar.Posicion_dia]
                    except ValueError:
                        pass
                else:
                    self.limpiar()
                    self.Guardar.datos_estadisticos = self.Datos_estadisticos.copy()
                    self.Guardar.Guardar_datos()
                return True
            else:
                return False
        return False
    
    def lista_elementos_calculos(self) -> None:
        if self.Datos_estadisticos.count(0) != 0:
            for element in range(self.Datos_estadisticos.count(0)):
                self.Datos_estadisticos.pop(self.Datos_estadisticos.index(0))

    def recolectar_datos(self, validacion: bool) -> dict:
        if validacion:
            if (len(self.data) == 6):
                self.Guardar.datos_arduino = {
                    "Tara": self.data[0],
                    "Peso Total": self.data[1],
                    "Radio llave": self.data[2],
                    "Suma Valores": self.data[3],
                    "Caudal Total": self.data[4],
                    "Altura Maxima": self.data[5]
                }
                self.Guardar.configuracion = self.Arduino.get_settings()
                self.Guardar.Guardar_datos()
                self.limpiar()
            else:
                print("Sin datos")
        return self.Guardar.datos_arduino

    def set_the_time(self, value: int) -> None:
        self.Guardar.tiempo = value
        self.Guardar.Guardar_datos()

    def leer_valores_celdad_carga(self) -> list:
        aux_list_regre: list = []
        if(len(self.data) == 5):
            for element in self.data:
                try:
                    variable : float = float(element)
                    aux_list_regre.append(variable)
                except ValueError:
                    self.Errores = "Error en el recivimiento del mensaje."
                    pass
            else:
                self.__guardar_Value_x(aux_list_regre)
                self.limpiar()
                return aux_list_regre.copy()
        return aux_list_regre
    
    def __guardar_Value_x(self, lista: list ):
        self.Guardar.Resion_values_x = lista
        self.Guardar.Guardar_datos()

    def guardar_valores_regresion(self, lista : list) -> None:
        if len(lista) == 3:
            self.Guardar.Regresion_metodo = {
                "a0" : lista[0],
                "x1" : lista[1],
                "x2" : lista[2]
            }
            self.Guardar.Guardar_datos()
        else:
            pass
                

if __name__ == '__main__':
    logic = Arduino(9600)
    logic.comprobar_data()

    """
    aux = [1.23,4.55,8.99]
    for element in aux:
        logic.Regresion(element)
    """
