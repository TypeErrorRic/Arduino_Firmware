import serial
import keyboard as k

class Transfer():

    Arduino : serial.Serial

    def __new__(cls, port: str, baudios: int) -> object:
        try:
            cls.Arduino = serial.Serial()
            print("Se inicializo: {0}".format(cls.__name__))
            cls.Arduino.parity = serial.PARITY_EVEN
        except TimeoutError:
            print("No se pudo inicializar: {0}".format(cls.__name__))
        finally:
            print("Done")
            return super().__new__(cls)

    def __init__(self, port: str, baudios: int) -> None:
        self.Arduino.port = port
        self.Arduino.baudrate = baudios
        try:
            self.Arduino.open()
            print("Puerto conectado corretament_ {0}".format(self.Arduino.name))
        except TimeoutError:
            print("No se pudo conectar al puerto: {0}".format(self.Arduino.name))
        finally: 
            print("Estado de la coneción: {}".format(self.Arduino.cts))
    
    def __call__(self):
        return self.Arduino.name

    def set_up(self) -> dict:
        if(self.Arduino != None): return self.Arduino.get_settings()
        else: return dict()

    def loop(self) -> None:
        aux : bool = True
        aux2 : bool = True
        with self.Arduino as com:
            while(self.Arduino.is_open):
                if (k.is_pressed('1')):
                    if(aux == True):
                        print("1")
                        com.write(b'1')
                        aux = False;
                        aux2 = True
                elif (k.is_pressed('2')):
                    if (aux2 == True):
                        com.write(b'2')
                        print("2")
                        aux = True
                        aux2 = False
                elif k.is_pressed('ENTER'):
                    com.close()
        print("Fin de la comunicacion")


if __name__ == '__main__':
    arduino = Transfer('COM9',9600)
    print(arduino())
    arduino.loop()
