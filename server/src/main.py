from logica import Arduino
from botones import Botones
from Ventanas import Ventana
from conectado_datos import Conectado_datos

#Ventana principal:
Window = Ventana("Monitor de Control del Tanque")

def main():
    print("Inicializando el programa...")
    principal = Window.mostrar_ventana()
    caja = Window.crearcaja(0.01, 0.18)
    Window.scrow_bar(caja)
    print("Estableciendo conexión...")
    dispositivo = Arduino(9600)
    valores = Conectado_datos(Window, dispositivo, caja)
    valores.opciones()
    Opciones = Botones(Window, dispositivo, caja)
    Opciones.regresion(0.85,0.20,"380x480")
    Opciones.conection_arduino()
    principal.mainloop()

if __name__ == "__main__":
    main()