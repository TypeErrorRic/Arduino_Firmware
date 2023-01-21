"""
Está clase contiene todos los botones y operaciones necesarias para el correcto funcionamiento del monitor.
"""

import tkinter
from logica import Arduino
from Ventanas import Ventana
from Regresion import realizar


class Botones:
    def __init__(self, pantalla: Ventana, Dispostivo: Arduino, caja: tkinter.Listbox) -> None:
        self.pantalla: tkinter.Tk = pantalla.mostrar_ventana()
        self.dispositivo: Arduino = Dispostivo
        self.caja = caja
        self.ventanas: Ventana = pantalla
        self.values_x : list = []

    #Funcionalidades para realizar la regresion:
    def __boton_regresion(self, posx: float, posy: float, size: str) -> tkinter.Button:
        boton = tkinter.Button(self.pantalla, text="Regresion", command=lambda: self.__regresion(size))
        boton.place(relx=posx, rely=posy)
        return boton

    def ingresar_variables(self, posx: float, posy: float, width, variables: tkinter.StringVar, estado) -> None:
        if estado == "disabled" or estado == "abled":
            tkinter.Entry(self.pantalla, textvariable=variables,
                          width=width, state =estado)

    def __regresion(self, size: str) -> None:
        nueva = tkinter.Toplevel(background="#2a8d90")
        nueva.title("Regresion")
        nueva.geometry(size)
        nueva.resizable(False, False)
        self.ventanas.etiqueta_titulo(nueva, "Regresion")
        self.ventanas.etiqueta_titulo_movible2(
            nueva, "Tipos de regresion", 0.345, 0.1)
        self.ventanas.etiqueta_titulo_movible2(
            nueva, " Ingrese Altura de cada Nivel en el Tanque", 0.17, 0.22)
        # Opcion seleccionada:
        opcion = tkinter.IntVar()
        opcion2 = tkinter.IntVar()
        boton_opcione2: tkinter.Checkbutton
        Evaluacion : tkinter.StringVar = tkinter.StringVar()
        # botones de selecion:
        boton_opcione1 = tkinter.Checkbutton(
            nueva, text="Regresion cuadratica", variable=opcion, command=lambda: self.__estado(
                opcion, boton_opcione2, opcion2
            ), background=u'#2a8d90'
        )
        print(boton_opcione1.config('text')[-1])
        boton_opcione1.place(relx=0.15, rely=0.15)
        boton_opcione1.config(state="disabled")
        boton_opcione1.deselect()
        boton_opcione2 = tkinter.Checkbutton(
            nueva, text="Regresion lineal", variable=opcion2, command=lambda: self.__estado(
                opcion2, boton_opcione1, opcion
            ), background=u'#2a8d90'
        )
        boton_opcione2.config(state="disabled")
        boton_opcione2.place(relx=0.6, rely=0.15)
        boton_opcione2.deselect()
        # Botones de modificacion:
        # Crear etiquetas 1:
        self.ventanas.etiqueta_titulo_movible(
            nueva, "Nivel del 20%", 0.4, 0.29)
        etiqueta1, et1 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.34)
        tkinter.Button(nueva, text="Modificar 1", command=lambda: et1.config(
            state="normal")).place(relx=0.61, rely=0.335)
        etiqueta1.set(f"{self.dispositivo.Guardar.Regresion_values_y[0]}")
        # Crear etiquetas 2:
        self.ventanas.etiqueta_titulo_movible(
            nueva, "Nivel del 40%", 0.4, 0.39)
        etiqueta2, et2 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.44)
        tkinter.Button(nueva, text="Modificar 2", command=lambda: et2.config(
            state="normal")).place(relx=0.61, rely=0.435)
        etiqueta2.set(f"{self.dispositivo.Guardar.Regresion_values_y[1]}")
        # Crear etiquetas 3:
        self.ventanas.etiqueta_titulo_movible(
            nueva, "Nivel del 60%", 0.4, 0.49)
        etiqueta3, et3 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.54)
        tkinter.Button(nueva, text="Modificar 3", command=lambda: et3.config(
            state="normal")).place(relx=0.61, rely=0.535)
        etiqueta3.set(f"{self.dispositivo.Guardar.Regresion_values_y[2]}")
        # Crear etiquetas 4:
        self.ventanas.etiqueta_titulo_movible(
            nueva, "Nivel del 80%", 0.4, 0.59)
        etiqueta4, et4 = self.ventanas.crear_entry_secundaria(nueva, 0.25, 0.64)
        tkinter.Button(nueva, text="Modificar 4", command=lambda: et4.config(
            state="normal")).place(relx=0.61, rely=0.635)
        etiqueta4.set(f"{self.dispositivo.Guardar.Regresion_values_y[3]}")
        # Crear etiquetas 4:
        etiqueta5 : tkinter.StringVar = tkinter.StringVar()
        etiqueta5.set(str(
            self.dispositivo.Guardar.datos_arduino.setdefault("Altura Maxima"))
        )
        #Botones de modificacion:
        Realizar = tkinter.Button(
            nueva, text="Realizar", command=lambda: self.__regresion_datos(
                etiqueta1, etiqueta2, etiqueta3, etiqueta4, etiqueta5, pantalla=nueva, estado1=opcion, estado2=opcion2
            ), bg='lightgreen', state="disabled"
        )
        Realizar.place(relx=0.36, rely=0.73)
        # eliminar datos ingresados:
        reset = tkinter.Button(
            nueva, text="Reset", command=lambda: self.__reset(nueva), bg='indian red'
        )
        reset.place(relx=0.53, rely=0.73)
        reset.config(state="disabled")
        # Boton para empezar llenar:
        tkinter.Button(nueva, text= "Empezar Llenado", command=lambda: self.llenar_tanque(
            Realizar, nueva)).place(relx = 0.36,rely = 0.85)
    
    def llenar_tanque(self, Realizar: tkinter.Button, nueva: tkinter.Toplevel) -> None:
        # desactivar opciones por un cierto tiempo:
        self.dispositivo.limpiar()
        self.dispositivo.escribir_datos("[true/8]")
        self.pantalla.after(self.dispositivo.Guardar.tiempo, self.__activar_Valores, Realizar)
        for widget in nueva.winfo_children():
            if isinstance(widget, tkinter.Button):
                if widget.config("text")[-1] == "Empezar Llenado":
                    widget.config(state="disabled")
                elif widget.config("text")[-1] == "Realizar":
                    widget.config(background="lightgreen")
                elif widget.config("text")[-1] == "Reset":
                    widget.config(background="indian red")
                else:
                    widget.config(state="active")
            elif isinstance(widget, tkinter.Checkbutton):
                widget.config(state="active")
                widget.config(background=u"#2a8d90")

    def __activar_Valores(self, boton: tkinter.Button) -> None:
        self.values_x = self.dispositivo.leer_valores_celdad_carga()
        boton.config(state="active")
        if len(self.values_x) == 5:
            self.caja.insert(tkinter.END, "Se recivieron los datos.")
        else:
            self.caja.insert(tkinter.END, "No se recivieron los datos.")

    def __estado(self, estado: tkinter.IntVar, boton: tkinter.Checkbutton, estado2: tkinter.IntVar):
        if (estado.get() == 1 and estado2.get() == 1):
            boton.invoke()
        else:
            pass

    def __definir(self, estado1: tkinter.IntVar, estado2: tkinter.IntVar) -> str:
        if (estado1.get() == 1 and estado2.get() == 0):
            return "cuadratica"
        elif (estado1.get() == 0 and estado2.get() == 1):
            return "lineal"
        else:
            return ""

    def __regresion_datos(self, *args: tkinter.StringVar, pantalla: tkinter.Toplevel, estado1: tkinter.IntVar, estado2: tkinter.IntVar) -> None:
        lista: list = []
        try:
            if (estado1.get() == 0 and estado2.get() == 0):
                raise TypeError("No se ha selecionado el tipo de regresion.")
        except TypeError as msg:
            self.caja.insert(tkinter.END, f"{msg}")
        else:
            for indx, element in enumerate(args, start=0):
                try:
                    aux5: str = element.get()
                    if float(aux5) > 0:
                        lista.append(float(aux5))
                    else:
                        raise ValueError
                except ValueError:
                    self.caja.insert(
                        tkinter.END, f"Error en la lectura del dato # {indx+1} : {element.get()}")
                    self.caja.insert(tkinter.END, "Intente de nuevo")
                    break
                else:
                    if indx == 4 and len(self.values_x) == 5:
                        #list1: list = [-2, 1, 4, 3, 0]
                        ele_regr : dict = {}
                        regresion_final: list = []
                        aux: str = self.__definir(estado1, estado2)
                        regresion_final, ele_regr = realizar(aux, self.values_x, lista)
                        self.caja.insert(tkinter.END, "Parejas Peso - Altura:")
                        for key, value in ele_regr.items():
                            self.caja.insert(tkinter.END, f"{key} \t {value}")
                        self.dispositivo.guardar_valores_y(list(ele_regr.values()))
                        self.caja.insert(
                                tkinter.END, "Valores de la Regresion {}:".format(aux))
                        for element in regresion_final:
                            self.caja.insert(
                                tkinter.END, "{:.2f}".format(element))
                            self.dispositivo.Regresion(element)
                        self.pantalla.after(2000, self.valorar_regresion)
                        pantalla.destroy()
                    elif len(self.values_x) < 5:
                        for widget in pantalla.winfo_children():
                            if isinstance(widget, tkinter.Button):
                                if widget.config('text')[-1] == "Realizar":
                                    self.__activar_Valores(widget)
                                    break
                        self.caja.insert(tkinter.END, "Valores del eje x no han sido recividos aún completos")
                    else:
                        pass
    
    def valorar_regresion(self):
        if self.dispositivo.confirmar_regresion():
            self.caja.insert(tkinter.END, "Regresion configurada")
        else:
            self.caja.insert(tkinter.END, "Regresion No configurada")
    
    def regresion(self, posx: float, posy: float, size: str) -> None:
        self.__boton_regresion(posx, posy, size)

    def __reset(self, pantalla: tkinter.Toplevel) -> None:
        for widget in pantalla.winfo_children():
            if isinstance(widget, tkinter.Entry):
                if widget["state"] == "normal":
                    widget.delete(0, 'end')
                    widget.insert(0, '0')
            elif isinstance(widget, tkinter.Checkbutton):
                widget.deselect()

    #Funcionalidades para relizar la conexión con el arduino:
    def conection_arduino(self) -> None:
        #Variables: Conexión:
        var = tkinter.StringVar(value="Automatico")
        var2 = tkinter.StringVar(value="Conectado")
        port: list = self.dispositivo.list_port()
        port.insert(0,"Automatico")
        self.ventanas.etiqueta_titulo_movible_main("Estado:", 0.01, 0.1)
        self.ventanas.etiqueta_titulo_movible_main("Puerto:", 0.24, 0.1)
        #Operaciones:
        aux = self.menu_opciones_conexion(var, port)
        etiqueta = self.ventanas.etiqueta_titulo_movible_main_cambiar(0.090, 0.1, "lightgreen", var2)
        #Botones para realizar la conexión y refrescar:
        boton1 =tkinter.Button(self.pantalla, text="Conectar", command=lambda: self.__conectar(
            var, etiqueta, var2))
        boton1.place(relx=0.55, rely=0.11)
        boton2 = tkinter.Button(self.pantalla, text="Refrescar", command=lambda: self.__refrescar(
            aux, etiqueta, var2, var, port))
        boton2.place(relx=0.64, rely=0.11)
        #Inicializaciones
        if not self.dispositivo.estado():
            self.__refrescar(aux, etiqueta, var2, var, port)
        else:
            boton1.config(state="disabled")
            aux.config(state="disabled")
            var.set(self.dispositivo.Port())
            self.caja.insert(tkinter.END, self.dispositivo.conexion)
            self.dispositivo.limpiar()
    
    #Boton para obtener las opciones:
    def menu_opciones_conexion(self, var: tkinter.StringVar, port: list) -> tkinter.OptionMenu:
        opciones = tkinter.OptionMenu(self.pantalla, var, *port)
        opciones.config(width=20)
        opciones.place(relx=0.32, rely=0.1)
        return opciones
    
    #Función conexión:
    def __conectar(self, var: tkinter.StringVar, etiqueta: tkinter.Label, var2: tkinter.StringVar) -> None:
        aux: str = var.get()
        if aux == "Automatico":
            aux = ""
        self.dispositivo.Reconectar(aux)
        if self.dispositivo.estado() and self.dispositivo.estado_conexion():
            var.set(self.dispositivo.Port())
            etiqueta.config(bg="lightgreen")
            var2.set("Conectado")
            for widget in self.pantalla.winfo_children():
                if isinstance(widget, tkinter.Button):
                    if widget.config('text')[-1] == "Conectar":
                        widget.config(state="disabled")
                    elif widget.config('text')[-1] == "Enviar datos":
                        widget.config(state="disabled")
                    else:
                        widget.config(state="active")
                elif isinstance(widget, tkinter.OptionMenu):
                        widget.config(state="disabled")
                elif isinstance(widget, tkinter.Entry):
                    widget.config(state="disabled")
            self.caja.insert(tkinter.END, self.dispositivo.conexion)
            self.dispositivo.limpiar()
        else:
            self.caja.insert(tkinter.END, self.dispositivo.Errores)
    
    #Función refrescar:
    def __refrescar(self, port: tkinter.OptionMenu, etiqueta: tkinter.Label, var: tkinter.StringVar, var2: tkinter.StringVar, puerto: list) -> None:
        if not self.dispositivo.estado_conexion():
            if etiqueta.config('text')[-1] == "Conectado":
                etiqueta.config(bg="indian red")
                var.set("Desconectado")
                for widget in self.pantalla.winfo_children():
                    if isinstance(widget, tkinter.Button):
                        if widget.config('text')[-1] == "Conectar":
                            widget.configure(state="active")
                        elif widget.config('text')[-1] == "Refrescar":
                            widget.configure(state="active")
                        else:
                            widget.configure(state="disabled")
                    elif isinstance(widget, tkinter.OptionMenu):
                        widget.config(state="active")
                    elif isinstance(widget, tkinter.Entry):
                        widget.config(state="disabled")
            else:
                aux: list = self.dispositivo.Puerto_disponibles()
                try:
                    puerto.pop(puerto.index("Automatico"))
                except ValueError:
                    pass
                puerto.extend(aux)
                elemento : set = set(puerto)
                puerto = list(elemento)
                puerto.insert(0, "Automatico")
                valor_aux: str = var2.get()
                self.__add(var2, port, puerto, valor_aux)
                self.caja.insert(tkinter.END, "Se Buscaron Nuevos Puertos.")
        elif self.dispositivo.estado_conexion():
            self.dispositivo.escribir_datos("[true/5]")
            self.pantalla.after(2000, self.confirmar)
            self.caja.delete(0,tkinter.END)
    
    def confirmar(self):
        if self.dispositivo.conection("sigue"):
            self.caja.insert(tkinter.END, "Conection True")
        else:
            self.dispositivo.desconectar()
            self.caja.insert(tkinter.END, "Conection False")

    def __add(self, var: tkinter.StringVar, menu: tkinter.OptionMenu, port: list, value: str):
        menu['menu'].delete(0, 'end')
        for opt in port: 
            menu['menu'].add_command(label=opt, command=tkinter._setit(var, opt))
        var.set(value)
    
    