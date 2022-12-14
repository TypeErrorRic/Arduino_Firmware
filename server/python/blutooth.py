import serial
import keyboard as k

class conectiones:
    velocidad = 9600

    def __init__(self, port) -> None:
        self.port = port

    def set_up(self) -> serial:
        try:
            comunicacion = serial.Serial(
                self.port, self.velocidad, timeout=5, parity=serial.PARITY_EVEN, rtscts=1)
            print(comunicacion.name)
            return comunicacion
        except TimeoutError:
            print("Error en conection.")
        finally:
            print("Done")

    def loop(self) -> None:
        ser = self.setup()
        while True:
            if k.is_pressed('y'):
                ser.write(b'y')
            if k.is_pressed('n'):
                ser.write(b'n')
            elif k.is_pressed('ENTER'):
                self.salir(ser)
                break
    
    def salir(self, ser) -> None:
        try:
            ser.close()
        except:
            print("Error en la cerradura del puerto")
        print("Conection Terminada")
