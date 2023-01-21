import tkinter
from logica import Arduino
from Ventanas import Ventana
from statistics import mode, median, mean

class Conectado_datos:

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
        self.Datos_Arduino_final: bool = True
        self.Estadistica: bool = True

    def etiquetas(self) -> None:
        self.ventanas.etiqueta_titulo_movible_main2(
            "Menu de Opciones", 0.79, 0.1)
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
        self.cerrar_all()
        self.confi_regresion()
        self.reiniciar()

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
        if self.Datos_Arduino_final:
            self.dispositivo.data.clear()
            self.dispositivo.escribir_datos("[true/6]")
            self.caja.insert(tkinter.END, "Datos cargados desde el arduino")
        else:
            self.caja.insert(tkinter.END, "Datos Recuperados")
        self.dispositivo.limpiar()
        self.pantalla.after(1400, self.__data_arduino)
    
    def __data_arduino(self) -> None:
        aux16: dict = self.dispositivo.recolectar_datos(self.Datos_Arduino_final)
        self.Datos_Arduino_final = False
        for key, value in aux16.items():
            self.caja.insert(tkinter.END, f"{key} : {value}")
    
    def estadistica(self) -> None:
        boton = tkinter.Button(
            self.pantalla, text="Estadistica", command= self.conseguir)
        boton.place(relx=0.843, rely=0.48)

    def conseguir(self) -> None:
        if self.Estadistica:
            self.dispositivo.escribir_datos("[true/3]")
        self.caja.insert(tkinter.END, "Datos en litros de la cantidad de jugo de caña de los ultimos 30 días")
        Estadistica_x2 : tkinter.Toplevel = tkinter.Toplevel(background="#2a8d90")
        Estadistica_x2.title("Estadistica")
        Estadistica_x2.geometry("380x180")
        Estadistica_x2.resizable(False, False)
        self.pantalla.after(2500, self.datos_dias, Estadistica_x2)
        #Etiqueta:
        self.ventanas.etiqueta_titulo(Estadistica_x2,"Medidas de Tendencia Central")
        #Botones:
        boton1 = tkinter.Button(Estadistica_x2, text ="Mediana", command=self.mediana, state="disabled")
        boton1.place(relx=0.1, rely=0.5)
        boton2 = tkinter.Button(
            Estadistica_x2, text="Media", command=self.media_ardu, state="disabled")
        boton2.place(relx=0.3, rely=0.5)
        boton3 = tkinter.Button(
            Estadistica_x2, text="Moda", command=self.moda, state="disabled")
        boton3.place(relx=0.5, rely=0.5)
        boton4 = tkinter.Button(
            Estadistica_x2, text="Imprimir datos", command=self.imprimir_datos_dias, state="disabled")
        boton4.place(relx=0.7, rely=0.5)
        boton5 = tkinter.Button(
            Estadistica_x2, text="Borrar datos", command=self.borrar_datos_estadisticos, state="disabled")
        boton5.place(relx=0.4, rely=0.8)

    def media_ardu(self) -> None:
        aux : float = mean(self.dispositivo.Datos_estadisticos)
        self.caja.insert(tkinter.END, f"La media de los datos es: {aux}")

    def moda(self) -> None:
        aux: float = mode(self.dispositivo.Datos_estadisticos)
        self.caja.insert(tkinter.END, f"La moda de los datos es: {aux}")

    def mediana(self) -> None:
        aux: float = median(self.dispositivo.Datos_estadisticos)
        self.caja.insert(tkinter.END, f"La mediana de los datos es: {aux}")

    def imprimir_datos_dias(self) -> None:
        self.caja.insert(tkinter.END, f"Los datos de los dias son: ")
        for indx, item in enumerate(self.dispositivo.Guardar.datos_estadisticos, start=1):
            self.caja.insert(tkinter.END, f"Día {indx}: {item}")

    def borrar_datos_estadisticos(self) -> None:
        self.dispositivo.escribir_datos("[true/4]")
        self.Estadistica = True
        self.pantalla.after(2000, self.datos_dias)

    def datos_dias(self, pantalla: tkinter.Toplevel) -> None:
        if self.dispositivo.datos_estadisticos_ard(self.Estadistica):
            self.caja.insert(tkinter.END, "Dato recividos correctamente")
            self.Estadistica = False
            for widget in pantalla.winfo_children():
                if isinstance(widget, tkinter.Button):
                    widget.config(state="active")
            self.dispositivo.lista_elementos_calculos()
        else:
            if not self.Estadistica:
                self.caja.insert(tkinter.END, "Dato ya pedidos")
                for widget in pantalla.winfo_children():
                    if isinstance(widget, tkinter.Button):
                        widget.config(state="active")
                self.dispositivo.lista_elementos_calculos()
            else:
                self.caja.insert(tkinter.END, "Error en el recivimiento de los datos")

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
            self.Datos_Arduino_final = True
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
        self.Datos_Arduino_final = True
        self.etiqueta.delete(0, 'end')
        self.__abilitar()
        self.pantalla.after(1200, self.confirmar_datos_arduino)

    def confirmar_datos_arduino(self):
        if self.__var == "peso":
            if self.dispositivo.conection(str(self.peso)):
                self.caja.insert(tkinter.END, "Dato enviado correctamente")
            else:
                self.caja.insert(tkinter.END, "Error enviando dato")
        elif self.__var == "altura":
            if self.dispositivo.conection(str(self.altura)):
                self.caja.insert(tkinter.END, "Dato enviado correctamente")
            else:
                self.caja.insert(tkinter.END, "Error enviando dato")
        self.peso = 0
        self.altura = 0
        self.__var = ""
    
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
        
    def cerrar_all(self) -> None:
        self.pantalla.protocol("WM_DELETE_WINDOW", self.cerrar_principal)

    def cerrar_principal(self) -> None:
        if self.dispositivo.estado_conexion():
            self.dispositivo.escribir_datos("[false/5]")
        self.pantalla.after(500, self.terminado)
    
    def terminado(self) -> None:
        self.dispositivo.desconectar()
        self.pantalla.destroy()
    
    def confi_regresion(self) -> None:
        boton = tkinter.Button(self.pantalla, text= "Configuracion", command=self.realizar_configuracion)
        boton.place(relx=0.83, rely=0.84)
    
    def realizar_configuracion(self) -> None:
        nueva : tkinter.Toplevel = tkinter.Toplevel(background="#2a8d90")
        nueva.geometry("380x120")
        nueva.resizable(False, False)
        nueva.title("Set the time")
        self.ventanas.etiqueta_titulo(nueva, "Tiempo de Llenado Tanque")
        variable = tkinter.StringVar(value="En segundos")
        etiqueta = tkinter.Entry(nueva, width=35, textvariable=variable)
        etiqueta.place(relx=0.1, rely=0.4)
        boton = tkinter.Button(nueva, text="Guardar", command=lambda:self.configuracion_tiempo(variable,etiqueta,nueva))
        boton.place(relx=0.7, rely=0.4)
        boton1 = tkinter.Button(nueva, text="Imprimir", command=self.mostrar_confi)
        boton1.place(relx = 0.4, rely = 0.7)
    
    def configuracion_tiempo(self, var: tkinter.StringVar, eti: tkinter.Entry, pantalla: tkinter.Toplevel) -> None:
        guarda: float
        try:
            guarda = float(var.get())
        except:
            eti.delete(0, 'end')
            var.set("")
            self.caja.insert(tkinter.END, "Dato no valido. Ingrese otro valor.")
        else:
            aux_21 : int = int(guarda*1000)
            self.dispositivo.set_the_time(aux_21)
            self.caja.insert(tkinter.END, "Dato configurado correctamente.")
            pantalla.destroy()
        
    def mostrar_confi(self) -> None:
        self.caja.insert(tkinter.END, "Configuración del dispostivio:")
        for key, value in self.dispositivo.Guardar.configuracion.items():
            self.caja.insert(tkinter.END, f"{key}: {value}")
        self.caja.insert(tkinter.END, f"tiempo de llenado: {self.dispositivo.Guardar.tiempo}")
    
    def reiniciar(self) -> None:
        boton = tkinter.Button(self.pantalla, text = "Reiniciar",
            background="indian red", command= lambda: self.__reset_all(), height=1)
        boton.place(relx=0.03, rely=0.03)

    def __reset_all(self) -> None:
        self.caja.insert(tkinter.END, "Reiniciar.")
        
        




