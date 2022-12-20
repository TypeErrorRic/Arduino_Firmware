import socket
import time

def Validacion():
    def wrapper(cls):
        class ejecucuion(cls):
            def loop(self):
                with super().sock_com() as com: 
                    while super().Update(com): pass
        return ejecucuion
    return wrapper


@Validacion()
class server():

    conexion : socket.socket
    addr : str = ""
    buffer : int = 1024
    _isinstance = None

    def __new__(cls) -> object:
        if cls._isinstance is None:
            try:
                my_socket =socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                my_socket.bind(('localhost', 8000))
                my_socket.listen(5)
                cls.conexion, cls.addr = my_socket.accept()
                print("Se inicializo el servidor")
            except TimeoutError:
                print("El tiempo de conexión se expiro")
        _isinstance = super().__new__(cls)
        return _isinstance

    def __init__(self) -> None:
        self._recibir = None
        print(f"Nueva conexion establecida en la dirreción {self.addr}")

    def __del__(self) -> None:
        self.conexion.close()

    @property ##getter
    def recibir(self) -> bytes:
        return self.recibir

    @recibir.setter #Permite establecer un valor
    def recibir(self, data: bytes) -> None:
        self.recibir = data

    def Update(self, con: socket.socket) -> bool:
        self.recibir = con.recv(self.buffer)
        if(self.recibir != b'1234'):
            print(self.recibir)
            self.enviar(con)
            return True
        else: return False

    def enviar(self, con: socket.socket) -> None:
        aux : bytes = bytes(input("Escriba: "), 'utf-8')
        con.sendall(aux)
    
    def sock_com(self) -> socket.socket:
        return self.conexion

if __name__ == "__main__":
    servidor = server()
    servidor.loop()