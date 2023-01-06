from module.Transfer import Transfer
from module.Base_datos import Almacenamiento
import time

class Arduino(Transfer):

    Guardar : Almacenamiento = Almacenamiento()
    Num_sensors : int = 5
    port_list : list = []

    def __new__(cls, baudios: int, listas: list = [], variable: str = ""):
        cls.port_list = listas
        return super().__new__(cls, baudios, variable)

    def __init__(self, baudios: int, listas: list = [], variable: str = ""):
        super().__init__(baudios, variable)
        self.__Altura : list = []
        self.__Regresion : dict = {}
        self.__contador: int = 0
        self.__contador2: int = 0
        self.__Peso_total: int = 0
        self.__Altura_max: int = 0

    @property
    def Altura(self) -> list:
        return self.__Altura
    
    @Altura.setter
    def Altura(self, value : float) -> None:
        if (contador := self.__contador + 1) <= (self.Num_sensors + 1):
            self.__Altura.append(value)
            self.__contador = contador
    
    @Altura.deleter
    def Altura(self) -> None:
        if len(self.__Altura) != 0:
            self.__Altura.pop(self.__contador - 1)
    
    def __Enviar_valores_regresion(self, lista : list) -> None:
        aux : str = "["
        for element in lista:
            aux += "{:.2}/".format(element)
        aux = aux + "0]"
        print(aux)
        super().escribir_datos(aux)

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
    
    def Datos_Almacenados(self, peso: float, maxi : float) -> None:
        aux: dict = {}
        lista_valores : str = "["
        aux.setdefault("Peso max", peso)
        lista_valores += f"{peso}/"
        aux.setdefault("Altura max", maxi)
        lista_valores += f"{maxi}/1]"
        self.Guardar.configuracion = aux
        #super().escribir_datos(lista_valores)

    def conection(self, value: str) -> bool:
        if self.Arduino.is_open:
            if self.señal.isSet():
                for intem in self.data:
                    if intem == value:
                        self.data.clear()
                        return True
                return False
            else:
                self.Arduino.close()
                return False
        else:
            return False
    
    def limpiar(self) -> None:
        if(len(self.data) != 0):
            self.data.clear()
        else:
            pass
    def mostrar_datos_en_buffer(self) -> None:
        if(len(self.data) == 0):
            print("sin datos")
        else:
            for item in self.data:
                print(item)

if __name__ == '__main__':
    logic = Arduino(9600)

    """
    aux = [1.23,4.55,8.99]
    for element in aux:
        logic.Regresion(element)
    """