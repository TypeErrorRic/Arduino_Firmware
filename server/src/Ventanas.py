import tkinter
#from logica import Arduino
class Ventana:
    def __init__(self, mensaje: str) -> None:
        self.ventana = tkinter.Tk()
        self.ventana.geometry("850x500")
        self.color: str = '#2a8d90'
        self.ventana.config(bg=self.color)
        self.Nombre = mensaje
        self.ventana.resizable(False, False)

    def set_up(self) -> None:
        tkinter.Label(self.ventana, text=self.Nombre,
                      bg=self.color, font="Times 20 bold").pack()

    def __crearcaja(self) -> tkinter.Listbox:
        self.listbox = tkinter.Listbox(self.ventana)
        self.listbox.place(relx=0.01, rely=0.1, relwidth=0.7, relheight=0.7)
        return self.listbox

    def __crear_venta_secundaria(self) -> None:
        

    def estructura_ingresar_datos(self, caja: tkinter.Listbox) -> None:
        datos = tkinter.StringVar()
        label = tkinter.Label(self.ventana, text="Escriba:",
                              font="Helbetica 10 bold", bg=self.color)
        label.place(relx=0.01, rely=0.85)
        self.__crear_entry(73, datos).place(relx=0.080, rely=0.85)
        self.__crearboton("Enviar datos", datos, caja).place(
            relx=0.63, rely=0.85)
        print("Hola")

    def loop(self) -> None:
        caja = self.__crearcaja()
        self.mostrar_datos(caja)
        print("hola")
        #self.__crearboton("presiona").place(relx = 0.8, rely=0.2)
        self.estructura_ingresar_datos(caja)
        self.ventana.mainloop()


if __name__ == "__main__":
    mostrar = Ventana("Configuracion")
    mostrar.loop()