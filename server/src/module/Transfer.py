#Clase: Transfer.
    #Está clase se usa para iniciailizar el puerto serial de comunicacion entre el arduino y el computador.
    #Conteniendo las operaciones de lectura y escritura de datos.

#Librerias:
import serial, serial.tools.list_ports
from threading import Thread, Event

class Transfer():

    Arduino : serial.Serial
    Ports : list = []
    Errores: str = ""
    baudios: int = 0
    conexion:  str = ""
    puerto: str = ""

    def __new__(cls, baudios: int, com: str = "") -> object:
        if(len(cls.Ports) == 0):
            cls.Ports= ['COM%s' % (i + 1) for i in range(21)]
        if(com == ""):
            aux: list = []
            for puerto in cls.Ports:
                try:
                    if (puerto == 'COM21'):
                        raise TimeoutError("No se encontro el puerto correcto.")
                    else:
                        cls.Arduino = serial.Serial(puerto, baudios)
                        if(cls.Arduino.is_open):
                            aux.append(puerto)
                            cls.Arduino.timeout = 2
                            mensaje: str = ""
                            mensaje = str(serial.to_bytes(cls.Arduino.read(10)), encoding='utf-8')
                            mensaje = mensaje[1:]
                            if (mensaje == "H05ricF45"):
                                break
                            else:
                                cls.Arduino.close()
                                continue
                except serial.SerialException:
                    pass
                except TimeoutError as msg:
                    cls.Errores = f"{msg}"
            cls.Ports = aux.copy()
        else:
            try:
                cls.Arduino = serial.Serial(com,baudios)
                if(cls.Arduino.is_open):
                    cls.Arduino.timeout = 5
                    mensaje: str = ""
                    mensaje = str(serial.to_bytes(cls.Arduino.read(10)), encoding='utf-8')
                    mensaje = mensaje[1:]
                    if (mensaje == "H05ricF45"):
                        pass
                    else:
                        cls.Arduino.close()
                        raise TimeoutError ("Conexión fallida")
                else:
                    raise serial.SerialException ("Puerto no abierto correctamente")
            except TimeoutError as msg:
                cls.Errores = f"{msg}"
            except serial.SerialException as msg:
                cls.Errores = f"{msg}"
        if(cls.Arduino.is_open):
           cls.conexion = "Se inicializo: {0}, en el puerto: {1}".format("Modulo",cls.Arduino.port)
        return super().__new__(cls)

    def __init__(self, baudios: int, com: str = "") -> None:
        self.mensaje: str = com
        self.__baudios = baudios
        self.señal = Event()
        self.hilo = None
        self.__data : list = []
        if(self.Errores == ""):
            try:
                if(self.Arduino.is_open):
                    self.Arduino.reset_input_buffer()
                    self.Arduino.write(b"HGTCh4rGu")
                    self.mensaje = str(serial.to_bytes(
                        self.Arduino.readline()), encoding='utf-8').rstrip('\n').rstrip('\r')
                    assert (self.mensaje == "HGTCh4rGu"), "Clave incorrecta"
                else:
                    raise TimeoutError("Puerto no abierto correctamente")
            except serial.SerialException:
                self.Errores = "Error en la lecutra del puerto"
                self.Arduino.close()
            except AssertionError as msg:
                self.Errores = f"{msg}"
                self.Arduino.close()
            except TimeoutError as msg:
                self.Errores = f"{msg}"
                self.Arduino.close()
            else:
                print("Inicializado correctamente")
                self.Ports.pop(self.Ports.index(self.Arduino.name))
                self.puerto = f"{self.Arduino.name}"
                self.inicializar_hilo()
    
    def inicializar_hilo(self):
        self.hilo = Thread(target=self.leer_datos)
        self.hilo.setDaemon(True)
        self.señal.set()
        self.hilo.start()

    def __stop_hilo(self):
        if(self.hilo is not None):
            self.señal.clear()
            self.hilo.join()
            self.hilo = None
    
    def desconectar(self):
        self.Arduino.close()
        self.__stop_hilo()

    def __call__(self) -> dict:
        return self.Arduino.get_settings()

    def __del__(self) -> None:
        self.desconectar()

    def leer_datos(self) -> None:
        try:
            while(self.señal.isSet() and self.Arduino.is_open):
                datos = self.Arduino.readline().decode("utf-8").strip()
                if(len(datos) > 0):
                    self.__data.append(datos)
        except TypeError:
            pass

    def escribir_datos(self, value: str) -> None:
        self.Arduino.reset_input_buffer()
        self.Arduino.reset_output_buffer()
        if(self.Arduino.is_open):
            datos = value
            self.Arduino.write(datos.encode())
        else:
            self.Errores = "No se pudo enviar el mensaje. Arduino desconectado."

    @property
    def data(self) -> list:
        return self.__data.copy()

    def comprobar_data(self) -> None:
        while True:
            print(self.__data)
            print(len(self.__data))
            aux = input("valor: ")
            if(aux == "1"):
                break
            if(aux == "0"):
                self.escribir_datos("[true/3]")
                print("escrito")
    
    def Reconectar(self, com:str) -> bool:
        if not self.Arduino.is_open:
            self.Errores = ""
            if(com == ""):
                aux: list = ['COM%s' % (i + 1) for i in range(21)]
                aux2: set = set(self.Ports) & set(aux)
                for element in aux2:
                    aux.pop(aux.index(element))
                for indx, puerto in enumerate(aux, start=1):
                    try:
                        if (indx == len(aux)):
                            raise TimeoutError("No se encontro el puerto correcto.")
                        else:
                            self.Arduino = serial.Serial(puerto, self.__baudios)
                            if (self.Arduino.is_open):
                                self.Ports.append(puerto)
                                self.Arduino.timeout = 5
                                mensaje: str = ""
                                mensaje = str(serial.to_bytes(self.Arduino.read(10)), encoding='utf-8')
                                mensaje = mensaje[1:]
                                if (mensaje == "H05ricF45"):
                                    break
                                else:
                                    self.Arduino.close()
                                    continue
                    except serial.SerialException:
                        pass
                    except TimeoutError as msg:
                        self.Errores = f"{msg}"
            else:
                try:
                    self.Arduino = serial.Serial(com, self.baudios)
                    if(self.Arduino.is_open):
                        self.Arduino.timeout = 5
                        mensaje: str = ""
                        mensaje = str(serial.to_bytes(self.Arduino.read(10)), encoding='utf-8')
                        mensaje = mensaje[1:]
                        if (mensaje == "H05ricF45"):
                            pass
                        else:
                            self.Arduino.close()
                            raise TimeoutError ("Conexión fallida")
                    else:
                        raise serial.SerialException ("Puerto no abierto correctamente")
                except TimeoutError as msg:
                    self.Errores = f"{msg}"
                except serial.SerialException as msg:
                    self.Errores = f"{msg}"
            if(self.Arduino.is_open):
                self.conexion = "Se inicializo: {0}, en el puerto: {1}".format("Modulo", self.Arduino.port)
                try:
                    if (self.Arduino.is_open):
                        self.Arduino.reset_input_buffer()
                        self.Arduino.write(b"HGTCh4rGu")
                        self.mensaje = str(serial.to_bytes(
                            self.Arduino.readline()), encoding='utf-8').rstrip('\n').rstrip('\r')
                        assert (self.mensaje == "HGTCh4rGu"), "Clave incorrecta"
                    else:
                        raise TimeoutError("Puerto no abierto correctamente")
                except serial.SerialException:
                    self.Errores = "Error en la lecutra del puerto"
                    self.Arduino.close()
                except AssertionError as msg:
                    self.Errores = f"{msg}"
                    self.Arduino.close()
                except TimeoutError as msg:
                    self.Errores = f"{msg}"
                    self.Arduino.close()
                else:
                    print("Inicializado correctamente")
                    self.inicializar_hilo()
                    self.puerto = f"{self.Arduino.name}"
                    return True
        else:
            pass
        return False
    
    def estado(self) -> bool:
        if self.Errores == "":
            return True
        else:
            self.Arduino.close()
            return False
    
    def Port(self) -> str:
        return self.puerto

    def list_port(self) -> list:
        return self.Ports.copy()
    
    def Puerto_disponibles(self) -> list:
        aux: list = [port.device for port in serial.tools.list_ports.comports()]
        comprobar: bool = False
        if(len(self.Ports) == 0):
            comprobar = True
        if not comprobar:
            aux2: set = set(self.Ports) & set(aux)
            for element in aux2:
                aux.pop(aux.index(element))
        return aux
    
    def estado_conexion(self) -> bool:
        if self.Arduino.is_open:
            if self.Errores == "":
                return True
            else:
                return False
        else:
            return False

    def limpiar(self) -> None:
        if (len(self.__data) != 0):
            self.__data.clear()
        else:
            pass

if __name__ == '__main__':
    arduino = Transfer(9600)
    aux = input("valor")
    arduino.Reconectar("")
    print(arduino.Errores)
    #arduino.comprobar_data()
    """
    print("//////////////////////////")
    print("Configuracion:")
    if arduino.estado():
        for llave, mensaje in arduino().items():
            print("{0}: {1}".format(llave,mensaje))
    else:
        print(arduino.Errores)
    aux :str = input("Ingrese: ")
    if aux == "1":
        arduino.Reconectar("")
        print(arduino.conexion)
    print(arduino.list_port())
    print(arduino.Puerto_disponibles())
    """