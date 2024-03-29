import json
from os import path as route
from os import scandir


class Almacenamiento():

    ruta_archivo : str = ""

    def __new__(cls) -> object:
        if(cls.ruta_archivo == ""):
            cls.ruta_archivo = route.dirname(__file__)
            eliminar: str = route.basename(cls.ruta_archivo)
            cls.ruta_archivo = cls.ruta_archivo[:cls.ruta_archivo.find(eliminar)]
            with scandir(cls.ruta_archivo) as ficheros:
                for fichero in ficheros:
                    if fichero.name == "Datos.json":
                        cls.ruta_archivo = route.join(cls.ruta_archivo, fichero.name)
        return super().__new__(cls)

    def __init__(self, configuracion : dict = {}) -> None:
        self.__configuracion = configuracion
        self.__datos_arduino : dict = {}
        self.__datos_estadisticos: list = []
        self.__guardar :dict = {}
        self.__tiempo_regr : int = 0
        self.__values_x : list = []
        self.__regresion: dict = {}
        self.__posicion: float = 0
        self.__alturas_y : list = []
    
    def Guardar_datos(self) -> None:
        self.__guardar = {
                            "configuracion": self.__configuracion, 
                            "Datos_Arduino": self.__datos_arduino, 
                            "Estadistica": self.__datos_estadisticos,
                            "Tiempo llenado": self.__tiempo_regr,
                            "Valores_x": self.__values_x,
                            "Regresion": self.__regresion,
                            "Posicion": self.__posicion,
                            "Valores_y" : self.__alturas_y
                        }
        with open(self.ruta_archivo, 'w') as archivo:
            json.dump(self.__guardar, archivo, indent=4)
    
    def Cargar_datos(self) -> None:
        with open(self.ruta_archivo, 'r') as archivo:
            self.__guardar = json.load(archivo)
            self.__configuracion = self.__guardar.setdefault("configuracion")
            self.__datos_arduino = self.__guardar.setdefault("Datos_Arduino")
            self.__datos_estadisticos = self.__guardar.setdefault("Estadistica")
            self.__tiempo_regr = self.__guardar.setdefault("Tiempo llenado")
            self.__values_x = self.__guardar.setdefault("Valores_x")
            self.__regresion = self.__guardar.setdefault("Regresion")
            self.__posicion = self.__guardar.setdefault("Posicion")
            self.__alturas_y = self.__guardar.setdefault("Valores_y")

    @property
    def configuracion(self) -> dict:
        return self.__configuracion

    @configuracion.setter
    def configuracion(self, dicionario: dict) -> None:
        self.__configuracion = dicionario

    @property
    def datos_arduino(self) -> dict:
        return self.__datos_arduino

    @datos_arduino.setter
    def datos_arduino(self, diccionario: dict) -> None:
        self.__datos_arduino = diccionario

    @property
    def datos_estadisticos(self) -> list:
        return self.__datos_estadisticos

    @datos_estadisticos.setter
    def datos_estadisticos(self, diccionario: list) -> None:
        self.__datos_estadisticos = diccionario

    @property
    def tiempo_regr(self) -> int:
        return self.__tiempo_regr
    
    @tiempo_regr.setter
    def tiempo(self, valor_time: int) -> None:
        self.__tiempo_regr = valor_time
    
    @property
    def Regresion_values_x_regr(self) -> list:
        return self.__values_x
    
    @Regresion_values_x_regr.setter
    def Regresion_values_x_regr(self, lista_values: list) -> None:
        self.__values_x.clear()
        for element in lista_values:
            self.__values_x.append(element)
    
    @property
    def Regresion_metodo(self) -> dict:
        return self.__regresion

    @Regresion_metodo.setter
    def Regresion_metodo(self, valor_dict: dict) -> None:
        self.__regresion = valor_dict

    @property
    def Posicion_dia(self) -> float:
        return self.__posicion
    
    @Posicion_dia.setter
    def Posicion_dia(self, value:float) -> None:
        self.__posicion = value

    @property
    def Regresion_values_y(self) -> list:
        return self.__alturas_y

    def __str__(self): 
        aux : str = ""
        for key, value in self.__guardar.items():
            if type(value) != dict:
                aux += f"{key}: \t {value} \n"
            elif type(value) == dict:
                aux += f"{key}: \n"
                for k, v in value.items():
                    if type(v) != dict:
                        aux += f" \t {k}: \t {v} \n"
                    elif type(v) == dict:
                        for i, j in value.items():
                            aux += f" \t \t {i}: \t {j} \n"
        return aux

if __name__ == '__main__':
    almacenar = Almacenamiento()
    almacenar.Cargar_datos()
    print(almacenar.datos_arduino)