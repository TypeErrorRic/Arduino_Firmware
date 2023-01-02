import tkinter

class Titulos:
    def __init__(self, pantalla: tkinter.Tk, titulo : str) -> None:
        self.pantalla = pantalla
        self.nombre = titulo

    def __inicializar(self) -> tkinter.Label:
        return tkinter.Label(self.pantalla, text = self.nombre, font = "Times 20")

    def etiqueta(self, posx : int, posy: int) -> None:
        self.__inicializar().place(relx = posx, rely = posy)
