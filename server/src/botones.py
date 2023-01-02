import tkinter
from logica import Arduino

class Botones:

    def __init__(self, pantalla: tkinter.Tk, nombre: str, client: Arduino, caja : tkinter.Listbox) -> None:
        self.pantalla = pantalla
        self.Nombre = nombre
        self.client = client
        self.caja = caja

    def boton_regresion(self, posx :int, posy: int) -> tkinter.Button:
        return tkinter.Button(self.pantalla, text = self.Nombre, command=lambda: self.__regresion()).place(relx=posx, rely = posy)

    def ingresar_variables(self, posx: int, posy: int, width, variables: tkinter.StringVar, estado: str) -> None:
        if estado == "disabled" or estado == "abled":
            tkinter.Entry(self.pantalla, textvariable=variables, width = width, state = estado)

    def __regresion(self) -> None:
        print("Hola")
    
    def enviar_datos(self, caja: tkinter.Listbox, variable: str):
        caja.insert(tkinter.END, f"Mi nombres: {variable}")