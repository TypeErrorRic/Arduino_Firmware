#Clase: Transfer.
    #Está clase se usa para iniciailizar el puerto serial de comunicacion entre el arduino y el computador.
    #Conteniendo las operaciones de lectura y escritura de datos.

#Librerias:
import serial

class Transfer():

    Arduino : serial.Serial
    Ports : list = []

    def __new__(cls, baudios: int, com: str = "") -> object:
        if(len(cls.Ports) == 0):
            cls.Ports = ['COM%s' % (i + 1) for i in range(21)]
        if(com == ""):
            for puerto in cls.Ports:
                try:
                    if (puerto == 'COM21'):
                        raise TimeoutError("No se encontro el puerto correcto.")
                    else:
                        cls.Arduino = serial.Serial(puerto, baudios)
                        if(cls.Arduino.is_open):
                            cls.Arduino.timeout = 5
                            mensaje: str = ""
                            mensaje = str(serial.to_bytes(cls.Arduino.read(9)), encoding='utf-8')
                            if (mensaje == "H05ricF45"):
                                break
                            else:
                                cls.Arduino.close()
                                continue
                except serial.SerialException:
                    pass
        else:
            cls.Arduino = serial.Serial(com,baudios)
            if(cls.Arduino.is_open):
                cls.Arduino.timeout = 5
                mensaje: str = ""
                mensaje = str(serial.to_bytes(cls.Arduino.read(10)), encoding='utf-8').rstrip('\n').rstrip('\r')
                if (mensaje == "H05ricF45"):
                    pass
                else:
                    raise TimeoutError ("Conexión fallida")
            else:
                raise serial.SerialException ("Puerto no abierto correctamente")
        if(cls.Arduino.is_open):
            print("Se inicializo: {0}, en el puerto: {1}".format(cls.__name__,cls.Arduino.port))
        return super().__new__(cls)

    def __init__(self, baudios: int, com: str = "") -> None:
        self.mensaje: str = com
        try:
            if(self.Arduino.is_open):
                self.Arduino.reset_input_buffer()
                self.Arduino.write(b"HGTCh4rGu")
                self.mensaje = self.leer_datos()
                assert (self.mensaje == "HGTCh4rGu"), "Clave incorrecta"
            else:
                raise TimeoutError("Puerto no abierto correctamente")
        except serial.SerialException:
            print("Error en la lecutra del puerto")
            self.Arduino.__del__()
        except AssertionError as msg:
            print(msg)
            self.Arduino.__exit__()
        except TimeoutError as msg:
            print(msg)
            self.Arduino.__exit__()
        else:
            print("Inicializado correctamente")
            self.Arduino.close()
    
    def __call__(self) -> dict:
        return self.Arduino.get_settings()

    def __del__(self) -> None:
        self.Arduino.__exit__()

    def leer_datos(self) -> str:
        return str(serial.to_bytes(self.Arduino.readline()), encoding='utf-8').rstrip('\n').rstrip('\r')

    def escribir_datos(self, value: str) -> None:
        enviar: bytes = bytes(value, 'utf-8')
        self.Arduino.write(enviar)
    
    def Puerto_conectado(self):
        return self.Arduino.name
    
    @staticmethod
    def Puerto_disponibles() -> list:
        lista_ports: list = []
        Transfer.Ports = ['COM%s' % (i + 1) for i in range(21)]
        for port in Transfer.Ports:
            try:
                Arduino = serial.Serial(port)
                if(Arduino.is_open):
                    lista_ports.append(port)
                Arduino.close()
            except serial.SerialException:
                pass
        return lista_ports


if __name__ == '__main__':
    arduino = Transfer(9600)
    print("//////////////////////////")
    print("Configuracion:")
    for llave, mensaje in arduino().items():
        print("{0}: {1}".format(llave,mensaje))
