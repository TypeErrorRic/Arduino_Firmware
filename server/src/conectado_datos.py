import tkinter
from logica import Arduino
from Ventanas import Ventana

class Conectado_datos:

    Estadistica: bool = False

    def __init__(self, pantalla: Ventana, Dispostivo: Arduino, caja: tkinter.Listbox) -> None:
        self.pantalla: tkinter.Tk = pantalla.mostrar_ventana()
        self.dispositivo: Arduino = Dispostivo
        self.caja = caja
        self.ventanas: Ventana = pantalla
        self.variable: tkinter.StringVar = tkinter.StringVar()
        self.etiqueta : tkinter.Entry
        self.__var : str = ""
        self.datos: list = []
        self.listas: list = []
        self.altura: float = 0
        self.peso: float = 0

    def etiquetas(self) -> None:
        self.ventanas.etiqueta_titulo_movible_main(
            "Menu de Opciones", 0.8, 0.1)
        self.etiqueta = tkinter.Entry(self.pantalla, textvariable=self.variable, width=80)
        self.etiqueta.place(relx=0.01, rely=0.9)
        self.etiqueta.config(state="disabled")
    
    def boton_enviar_datos(self) -> None:
        boton = tkinter.Button(self.pantalla, text="Enviar datos")
        boton.config(state="disabled")
        boton.place(relx=0.6, rely=0.90)

    def opciones(self) -> None:
        self.etiquetas()
        self.boton_enviar_datos()
        self.ingrear_peso()
        self.ingresar_alatura()
        self.descartar()
        self.guardar()
        self.obtener_datos()
        self.estadistica()
        self.celdad_carga()

    def ingrear_peso(self) -> None:
        boton = tkinter.Button(self.pantalla, text="Ing. Peso", command= lambda: self.__desactivar("peso"))
        boton.place(relx=0.85, rely=0.27)

    def ingresar_alatura(self) -> None:
        boton = tkinter.Button(
            self.pantalla, text="Ing. Altura", command= lambda: self.__desactivar("altura"))
        boton.place(relx=0.8465, rely=0.34)

    def obtener_datos(self) -> None:
        boton = tkinter.Button(
            self.pantalla, text="Datos", command=self.__pedir_datos)
        boton.place(relx=0.86, rely=0.41)

    def __pedir_datos(self) -> None:
        if not self.Estadistica:
            self.dispositivo.data.clear()
            self.dispositivo.escribir_datos("[true/6]")
            self.pantalla.after(1400, self.__data_arduino)
        self.caja.insert(tkinter.END, "Datos cargados desde el arduino")
    
    def __data_arduino(self) -> None:
        if(len(self.dispositivo.data) != 0):
            for item in self.dispositivo.data:
                self.listas.append(item)
            dictionary :dict = {
                "Tara": self.listas[0],
                "Peso Total" : self.listas[1],
                "Radio llave": self.listas[2],
                "Suma Valores": self.listas[3],
                "Caudal Total": self.listas[4],
                "Altura Maxima": self.listas[5]
            }
            self.dispositivo.limpiar()
        else:
            print("Sin datos")
    
    def estadistica(self) -> None:
        boton = tkinter.Button(
            self.pantalla, text="Estadistica", command= self.conseguir)
        boton.place(relx=0.843, rely=0.48)

    def conseguir(self) -> None:
        if not self.Estadistica:
            self.dispositivo.data.clear()
            self.dispositivo.escribir_datos("[true/3]")
            self.pantalla.after(2500, self.datos_dias)
        self.caja.insert(tkinter.END, "Datos en litros de la cantidad de jugo de caña de los ultimos 30 días")

    def datos_dias(self) -> None:
        if(len(self.dispositivo.data) != 0):
            for item in self.dispositivo.data:
                self.datos.append(item)
                print(item)
            self.dispositivo.limpiar()
        else:
            print("Sin_datos")

    def celdad_carga(self) -> None:
        boton = tkinter.Button(
            self.pantalla, text="Celdad Carga")
        boton.place(relx=0.835, rely=0.55)
        boton.config(command=lambda: self.__calibracion(boton))

    def __calibracion(self, boton : tkinter.Button) -> None:
        if not boton.config('text')[-1] == "Parar":
            for widget in self.pantalla.winfo_children():
                if isinstance(widget, tkinter.Button):
                    if widget.config('text')[-1] == "Celdad Carga":
                        widget.config(text = "Parar")
                        widget.place(relx=0.86, rely=0.55)
                    else:
                        widget.configure(state="disabled")
            self.dispositivo.escribir_datos("[true/2]")
        else:
            boton.config(text = "Celdad Carga")
            self.dispositivo.escribir_datos("ttttt")
            boton.place(relx=0.835, rely=0.55)
            self.__abilitar()

    def guardar(self) -> None:
        boton = tkinter.Button(
            self.pantalla, text="Guardar", command=self.__guardar_cambios)
        boton.place(relx=0.85, rely=0.7)
        boton.config(state="disabled")

    def __guardar_cambios(self) -> None:
        aux: str = ""
        if self.__var == "peso":
            aux = "[{0}/1]".format(self.peso)
            self.dispositivo.escribir_datos(aux)
        elif self.__var == "altura":
            aux = "[{0}/7]".format(self.altura)
            self.dispositivo.escribir_datos(aux)
        self.peso = 0
        self.altura = 0
        self.etiqueta.delete(0, 'end')
        self.dispositivo.limpiar()
        self.__abilitar()
    
    def descartar(self) -> None:
        boton = tkinter.Button(
            self.pantalla, text="Descartar", command= lambda: self.__descartar_cambios())
        boton.place(relx=0.8465, rely=0.77)
        boton.config(state="disabled")

    def __descartar_cambios(self) -> None:
        self.variable.set("")
        self.__var = ""
        self.__abilitar()
        self.etiqueta.delete(0, 'end')
        
    def __abilitar(self) -> None:
        for widget in self.pantalla.winfo_children():
            if isinstance(widget, tkinter.Button):
                if widget.config('text')[-1] == "Enviar datos":
                    widget.configure(state="disabled")
                    widget.config(command=lambda: print("hola"))
                elif widget.config('text')[-1] == "Conectar":
                    pass
                elif widget.config('text')[-1] == "Guardar":
                    widget.configure(state="disabled")
                elif widget.config('text')[-1] == "Descartar":
                        widget.configure(state="disabled")
                else:
                    widget.configure(state="active")
            elif isinstance(widget, tkinter.Entry):
                widget.config(state="disabled")
    
    def __desactivar(self, var: str) -> None:
        for widget in self.pantalla.winfo_children():
            if isinstance(widget, tkinter.Button):
                if widget.config('text')[-1] == "Enviar datos":
                    widget.configure(state="active")
                    if (var == "peso"):
                        self.caja.insert(tkinter.END, "Ingrese el peso total: ")
                        widget.config(command=self.__config_peso)
                    elif (var == "altura"):
                        self.caja.insert(tkinter.END, "Ingrese la altura maxima: ")
                        widget.config(command=self.__config_Altura)
                    else:
                        widget.config(command=lambda: print("hola"))
                else:
                    widget.configure(state="disabled")
            elif isinstance(widget, tkinter.Entry):
                widget.config(state="normal")
            
    def __config_peso(self) -> None:
        try:
            peso: float = float(self.variable.get())
        except ValueError:
            self.caja.insert(tkinter.END, "Dato ingresado es erroneo. Ingrese otro")
            self.etiqueta.delete(0, 'end')
        else:
            self.peso = peso
            self.caja.insert(tkinter.END, "El peso ingresado es: {}".format(peso))
            self.caja.insert(tkinter.END, "Deseala guardar o descartarla.")
            for widget in self.pantalla.winfo_children():
                if isinstance(widget, tkinter.Button):
                    if widget.config('text')[-1] == "Guardar":
                        widget.configure(state="active")
                    elif widget.config('text')[-1] == "Descartar":
                        widget.configure(state="active")
            self.__var = "peso"


    def __config_Altura(self) -> None:
        try:
            altura: float = float(self.variable.get())
        except ValueError:
            self.caja.insert(tkinter.END, "Dato ingresado es erroneo. Ingrese otro")
            self.etiqueta.delete(0, 'end')
        else:
            self.altura = altura
            self.caja.insert(
                tkinter.END, "La altura ingresada es: {}".format(altura))
            self.caja.insert(tkinter.END, "Deseala guardar o descartarla.")
            for widget in self.pantalla.winfo_children():
                if isinstance(widget, tkinter.Button):
                    if widget.config('text')[-1] == "Guardar":
                        widget.configure(state="active")
                    elif widget.config('text')[-1] == "Descartar":
                        widget.configure(state="active")
            self.__var = "altura"


