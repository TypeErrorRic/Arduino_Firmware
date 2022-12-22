import socket
import time

def Validacion():
    def wrapper(cls):
        class ejecucuion(cls):
            def loop(self):
                if super().verificacion(): 
                    print("Empiece a escribir")
                    with super().sock_com() as com: 
                        while super().Update(com): pass
                else:
                    print("salio")
                    super().__del__()
        return ejecucuion
    return wrapper


@Validacion()
class server():

    conexion : socket.socket
    addr : str = ""
    buffer : int = 1024
    _isinstance: object = None
    my_socket: socket.socket

    def __new__(cls, port: int) -> object:
        if cls._isinstance is None:
            cls.my_socket =socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            cls.my_socket.bind(('127.0.0.1', port))
            cls.my_socket.listen(5)
            cls.my_socket.settimeout(40)
            print("Se inicializo el servidor. Esperando a que se establesca la conexión...")
        cls._isinstance = super().__new__(cls)
        return cls._isinstance

    def __init__(self, port: int) -> None:
        try:
            self.conexion, self.addr = self.my_socket.accept()
            print(f"Nueva conexion establecida en la dirreción {self.addr}")
        except socket.timeout:
            print("Conexión no realizada")
            self._isinstance = None
        finally:
            print("Done")

    def __del__(self) -> None:
        if self._isinstance != None:
            self.conexion.close()
            self._isinstance = None
            print("Conexión terminada.")
        else:
            print("Conexión no realizada correctamente")

    def recibir(self, con: socket.socket) -> bytes:
        return con.recv(self.buffer)

    def Update(self, con: socket.socket) -> bool:
        self.enviar(con)
        return True

    def enviar(self, con: socket.socket) -> None:
        aux : bytes = bytes(input("Escriba: "), 'utf-8')
        con.sendall(aux)
    
    def sock_com(self) -> socket.socket:
        return self.conexion

    def verificacion(self) -> bool:
        if self._isinstance == None:
            return False
        else:
            return True

if __name__ == "__main__":
    servidor = server(27015)
    servidor.loop()