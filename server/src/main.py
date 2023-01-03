from logica import Arduino
from botones import Botones
from Ventanas import Ventana

Window = Ventana("Configuracion")

def mostrar() -> None:
    pass

def main():
    principal = Window.mostrar_ventana()
    caja = Window.crearcaja(0.01, 0.1)
    regresion = Botones(Window,"Regresion", caja)
    regresion.regresion(0.85,0.10,"380x480")
    principal.mainloop()

if __name__ == "__main__":
    main()