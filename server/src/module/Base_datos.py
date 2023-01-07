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
    
    def Guardar_datos(self) -> None:
        self.__guardar = {
                            "configuracion": self.__configuracion, 
                            "Datos_Arduino": self.__datos_arduino, 
                            "Estadistica": self.__datos_estadisticos
                        }
        with open(self.ruta_archivo, 'w') as archivo:
            json.dump(self.__guardar, archivo, indent=4)
    
    def Cargar_datos(self) -> dict:
        with open(self.ruta_archivo, 'r') as archivo:
            self.__guardar = json.load(archivo)
            self.__configuracion = self.__guardar.setdefault("configuracion")
            self.__datos_arduino = self.__guardar.setdefault("Datos_Arduino")
            self.__datos_estadisticos = self.__guardar.setdefault("Estadistica")
            return self.__guardar

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