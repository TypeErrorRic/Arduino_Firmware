import tkinter
from logica import Arduino
from Ventanas import Ventana
import time

class Botones:
    def __init__(self, pantalla: Ventana, nombre: str, caja : tkinter.Listbox) -> None:
        self.pantalla: tkinter.Tk = pantalla.mostrar_ventana()
        self.Nombre = nombre
        self.caja = caja
        self.ventanas: Ventana = pantalla

    def __boton_regresion(self, posx: int, posy: int, size: str) -> tkinter.Button:
        return tkinter.Button(self.pantalla, text = self.Nombre, command=lambda: self.__regresion(size)).place(relx=posx, rely = posy)

    def ingresar_variables(self, posx: int, posy: int, width, variables: tkinter.StringVar, estado: str) -> None:
        if estado == "disabled" or estado == "abled":
            tkinter.Entry(self.pantalla, textvariable=variables, width = width, state = estado)

    def __regresion(self, size: str) -> None:
        nueva = tkinter.Toplevel(background="#2a8d90")
        nueva.title("Regresion")
        nueva.geometry(size)
        nueva.resizable(False, False)
        self.ventanas.etiqueta_titulo(nueva, "Regresion")
        self.ventanas.etiqueta_titulo_movible2(nueva, "Tipos de regresion", 0.345, 0.1)
        self.ventanas.etiqueta_titulo_movible2(nueva, " Ingrese Altura de cada Nivel en el Tanque", 0.17, 0.22)
        #Opcion seleccionada:
        opcion = tkinter.IntVar()
        opcion2 = tkinter.IntVar()
        boton_opcione2: tkinter.Checkbutton
        #botones de selecion:
        boton_opcione1 = tkinter.Checkbutton(
            nueva, text="Regresion cuadratica", variable=opcion, command=lambda: self.__estado(opcion, boton_opcione2, opcion2),
            background=u'#2a8d90')
        boton_opcione1.place(relx=0.15, rely=0.15)
        boton_opcione1.deselect()
        boton_opcione2 = tkinter.Checkbutton(
            nueva, text="Regresion lineal", variable=opcion2, command=lambda: self.__estado(
                opcion2, boton_opcione1, opcion
                ), background=u'#2a8d90'
            )
        boton_opcione2.place(relx=0.6, rely=0.15)
        boton_opcione2.deselect()
        #Crear etiquetas 1:
        self.ventanas.etiqueta_titulo_movible(nueva, "Nivel del 20%", 0.4, 0.29)
        etiqueta1 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.34)
        #Crear etiquetas 2:
        self.ventanas.etiqueta_titulo_movible(nueva, "Nivel del 40%", 0.4, 0.39)
        etiqueta2 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.44)
        #Crear etiquetas 3:
        self.ventanas.etiqueta_titulo_movible(nueva, "Nivel del 60%", 0.4, 0.49)
        etiqueta3 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.54)
        #Crear etiquetas 4:
        self.ventanas.etiqueta_titulo_movible(nueva, "Nivel del 80%", 0.4, 0.59)
        etiqueta4 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.64)
         #Crear etiquetas 4:
        self.ventanas.etiqueta_titulo_movible(nueva, "Nivel del 90%", 0.4, 0.69)
        etiqueta5 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.74)
        Realizar = tkinter.Button(
            nueva, text="Realizar", command=lambda: self.__regresion_datos(
                etiqueta1, etiqueta2, etiqueta3, etiqueta4, etiqueta5, pantalla=nueva
            ), bg='lightgreen'
        )
        Realizar.place(relx=0.36, rely=0.85)
        #eliminar datos ingresados:
        reset = tkinter.Button(
            nueva, text="Reset", command=lambda: self.__reset(nueva), bg='indian red'
        )
        reset.place(relx = 0.53, rely = 0.85)
    
    def __estado(self, estado : tkinter.IntVar, boton: tkinter.Checkbutton, estado2 : tkinter.IntVar):
        if(estado.get() == 1 and estado2.get() == 1):
            boton.invoke()
        else:
            pass

    def __regresion_datos(self, *args, pantalla: tkinter.Toplevel) -> None:
        lista: list = []
        aux: float
        for indx, element in enumerate(args, start=0):
            try:
                aux = float(element.get())
                if(aux > 0):
                    lista.append(aux)
                else:
                    raise ValueError
            except ValueError:
                self.caja.insert(tkinter.END, f"Error en la lectura del dato # {indx} : {element.get()}")
                break
            else:
                if(indx == 4):
                    for element in lista:
                        self.caja.insert(tkinter.END, f"{element}")
                    pantalla.destroy()
                else:
                    pass

    def regresion(self, posx: int, posy: int, size: str) -> None:
        self.__boton_regresion(posx, posy, size)

    def __reset(self, pantalla: tkinter.Toplevel) -> None:
        for widget in pantalla.winfo_children():
            if isinstance(widget, tkinter.Entry):
                widget.delete(0, 'end')
                widget.insert(0,'0')
            elif isinstance(widget, tkinter.Checkbutton):
                widget.deselect()
    
    def enviar_datos(self, caja: tkinter.Listbox, variable: str):
        caja.insert(tkinter.END, f"Mi nombres: {variable}")
