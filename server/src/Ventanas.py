import tkinter

class Ventana:
    def __init__(self, mensaje: str) -> None:
        self.ventana = tkinter.Tk()
        self.ventana.geometry("850x500")
        self.color: str = '#2a8d90'
        self.ventana.config(bg=self.color)
        self.Nombre = mensaje
        self.ventana.resizable(False, False)
        tkinter.Label(self.ventana, text=self.Nombre,
                      bg=self.color, font="Times 20 bold").pack()
        self.width: int = 30

    def crearcaja(self, sizex: int, sizey: int) -> tkinter.Listbox:
        self.listbox = tkinter.Listbox(self.ventana)
        self.listbox.place(relx=sizex, rely=sizey, relwidth=0.7, relheight=0.7)
        return self.listbox
        
    def mostrar_ventana(self) -> tkinter.Tk:
        return self.ventana

    def color(self) -> str:
        return self.color
    
    def etiqueta_titulo(self, pantalla: tkinter.Toplevel, nombre: str) -> tkinter.Label:
        return tkinter.Label(pantalla, text=nombre, font="Times 20", bg = self.color).pack()
    
    def etiqueta_titulo_movible(self, pantalla: tkinter.Toplevel, nombre: str, posx: int, posy: int) -> tkinter.Label:
        return tkinter.Label(pantalla, text=nombre, font="Times 10", bg=self.color).place(relx=posx, rely=posy)
    
    def etiqueta_titulo_movible2(self, pantalla: tkinter.Toplevel, nombre: str, posx: int, posy: int) -> tkinter.Label:
        return tkinter.Label(pantalla, text=nombre, font="Times 12", bg=self.color).place(relx=posx, rely=posy)

    def crear_entry_secundaria(self, pantalla : tkinter.Toplevel, sizex : int, sizey: int) -> tkinter.StringVar:
        variable = tkinter.StringVar(value="0")
        tkinter.Entry(pantalla, width=self.width, textvariable=variable).place(relx=sizex, rely=sizey)
        return variable
        

if __name__ == "__main__":
    mostrar = Ventana("Configuracion")
    mostrar.loop()