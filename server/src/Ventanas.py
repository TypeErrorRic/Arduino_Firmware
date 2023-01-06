import tkinter

class Ventana:
    def __init__(self, mensaje: str) -> None:
        self.ventana = tkinter.Tk()
        self.ventana.geometry("850x500")
        self.__color: str = '#2a8d90'
        self.ventana.config(bg=self.__color)
        self.Nombre = mensaje
        self.ventana.resizable(False, False)
        tkinter.Label(self.ventana, text=self.Nombre,
                      bg=self.__color, font="Times 20 bold").pack()
        self.width: int = 30

    def crearcaja(self, sizex: float, sizey: float) -> tkinter.Listbox:
        listbox = tkinter.Listbox(self.ventana)
        listbox.place(relx=sizex, rely=sizey, relwidth=0.7, relheight=0.7)
        return listbox
        
    def mostrar_ventana(self) -> tkinter.Tk:
        return self.ventana

    def color(self) -> str:
        return self.__color
    
    def etiqueta_titulo_movible_main(self, nombre: str, posx: float, posy: float) -> tkinter.Label:
        label = tkinter.Label(self.ventana, text=nombre, font="Times 15", bg=self.__color)
        label.place(relx=posx, rely=posy)
        return label
    
    def etiqueta_titulo_movible_main_cambiar(self, posx: float, posy: float, color: str, var: tkinter.StringVar) -> tkinter.Label:
        variable = tkinter.Label(self.ventana, textvariable=var, font="Times 15")
        variable.config(bg = color)
        variable.place(relx=posx, rely=posy)
        return variable
    
    def etiqueta_titulo(self, pantalla: tkinter.Toplevel, nombre: str) -> tkinter.Label:
        label = tkinter.Label(pantalla, text=nombre, font="Times 20", bg = self.__color)
        label.pack()
        return label
    
    def etiqueta_titulo_movible(self, pantalla: tkinter.Toplevel, nombre: str, posx: float, posy: float) -> tkinter.Label:
        label = tkinter.Label(pantalla, text=nombre, font="Times 10", bg=self.__color)
        label.place(relx=posx, rely=posy)
        return label
    
    def etiqueta_titulo_movible2(self, pantalla: tkinter.Toplevel, nombre: str, posx: float, posy: float) -> tkinter.Label:
        label = tkinter.Label(pantalla, text=nombre, font="Times 12", bg=self.__color)
        label.place(relx=posx, rely=posy)
        return label

    def crear_entry_secundaria(self, pantalla : tkinter.Toplevel, sizex : float, sizey: float) -> tkinter.StringVar:
        variable = tkinter.StringVar(value="0")
        tkinter.Entry(pantalla, width=self.width, textvariable=variable).place(relx=sizex, rely=sizey)
        return variable
    
    def scrow_bar(self, listbox: tkinter.Listbox) -> None:
        scroll = tkinter.Scrollbar(self.ventana, command=listbox.yview)
        scroll.place(relx=0.73, rely=0.18,relheight=0.7)

        

if __name__ == "__main__":
    mostrar = Ventana("Configuracion")